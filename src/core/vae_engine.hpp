#ifndef _VAE_ENGINE
#define _VAE_ENGINE

#include "./vae_util.hpp"
#include "./vae_types.hpp"
#include "./vae_config.hpp"

#include "./device/vae_rtaudio.hpp"
#include "./device/vae_portaudio.hpp"
#include "./device/vae_device_dummy.hpp"

#include "./pod/vae_listener.hpp"
#include "./pod/vae_emitter.hpp"
#include "./vae_voice_manager.hpp"
#include "./vae_spatial_manager.hpp"
#include "./processor/vae_processor.hpp"
#include "./processor/vae_spatial_processor.hpp"
#include "./processor/vae_mixer_processor.hpp"
#include "./vae_bank_manager.hpp"

#include "../../external/tklb/src/util/TMath.hpp"
#include "./vae_logger.hpp"
#include "./voices/vae_voice_filter.hpp"

#include <cstring>				// strcmp
#include <mutex>
#include <thread>				// mAudioThread
#include <condition_variable>	// mAudioConsumed

/**
 * @brief Marks a function for export to the generated vae::EnginePimpl class
 * [generated from generate_pimpl.py](../../scripts/README.md)
 * @see vae::EnginePimpl
 */
#define _VAE_PUBLIC_API


namespace vae { namespace core {
	/**
	 * @brief Central class handling all outside communication.
	 * @details Can be sealed off by using EnginePimpl instead
	 */

	class Engine {
		using CurrentBackend = BackendRtAudio;

		using Thread = std::thread;
		using ConditionVariable = std::condition_variable;

		EngineConfig mConfig;			///< Config object provided at initlalization

		VoiceManger mVoiceManager;		///< Holds and handle voices
		SpatialManager mSpatialManager;	///< Holds and manages spatial emitters
		BankManager mBankManager;		///< Holds and manages banks

		Processor mProcessor;				///< Default Voice processor
		MixerProcessor mMixerProcessor;		///< Mixer channel processor
		SpatialProcessor mSpatialProcessor;	///< Spatial voice processor

		Device* mDevice = nullptr;		///< Output device
		AudioBuffer mScratchBuffer;		///< used to combine the signal from all banks and push it to the device
		SampleIndex mTime = 0;			///< Global engine time in samples
		Time mTimeFract = 0;			///< Global engine time in seconds
		Sample mLimiterLastPeak = 1.0;	///< Master limiter last peak
		Sample mMasterVolume = 1.0;		///< Master Colume applied after limiting

		Thread* mAudioThread;				///< Thread processing voices and mixers
		ConditionVariable mAudioConsumed;	///< Notifies the audio thread when more audio is needed
		std::mutex mMutex;					///< Mutex needed to use mAudioConsumed, doesn't actually do anything else
		bool mAudioThreadRunning = false;

		/**
		 * @brief Main processing function.
		 * @details Called either from onBufferSwap or threadedProcess
		 * This does't need to be locked since it only renders the banks.
		 * The data accessed from this class will only be used from this thread.
		 * The bank however needs to be locks which happens in the bankmanager
		 */
		void process() {
			VAE_PROFILER_FRAME_MARK_START(profiler::audioFrame)
			VAE_PROFILER_SCOPE_NAMED("Engine Process")
			auto& d = *mDevice;
			auto sampleRate = d.getSampleRate();

			const Time step = 1.0 / Time(sampleRate);

			// process until device can't take any more audio
			while (true) {
				// ! this is an underestimate when resampling so we don't have any leftovers
				auto remaining = d.canPush();

				static_assert(32 <= StaticConfig::MaxBlock, "MaxBlock needs to be larger");

				if (remaining < 32) {
					break; // ! Don't even bother with small blocks, we'll get em next time
				}

				// clamp to max processable size, the preallocated scratch buffers can't take any larger blocks
				remaining = std::min(remaining, StaticConfig::MaxBlock);

				mScratchBuffer.setValidSize(remaining);

				Size renderedNormal = 0;
				Size renderedSpatial = 0;
				// TODO PERF VAE banks could be processed in parallel
				// however each bank needs to get a temporary own copy of the voice
				// or else this will be false sharing city
				mBankManager.forEach([&](Bank& i) {
					renderedNormal += mProcessor.mix(mVoiceManager, i, remaining, sampleRate);
					renderedSpatial += mSpatialProcessor.mix(
						mVoiceManager, i, mSpatialManager, remaining, sampleRate
					);
					mMixerProcessor.mix(mVoiceManager, i, remaining);
					auto& bankMaster = i.mixers[Mixer::MasterMixerHandle].buffer;
					mScratchBuffer.add(bankMaster);
					bankMaster.set(0);
				});

				VAE_PROFILER_PLOT("Rendered Normal Voices", int64_t(renderedNormal));
				VAE_PROFILER_PLOT("Rendered Spatial Voices", int64_t(renderedSpatial));
				VAE_PROFILER_PLOT("Rendered Total Voices", int64_t(renderedSpatial + renderedNormal));

				{
					VAE_PROFILER_SCOPE_NAMED("Peak limiting")
					// Shitty peak limiter
					mLimiterLastPeak *= Sample(0.7); // return to normal slowly
					mLimiterLastPeak = std::max(Sample(1.0), mLimiterLastPeak);
					Sample currentPeak = 0;
					for (Uchar c = 0; c < mScratchBuffer.channels(); c++) {
						for (Size i = 0; i < mScratchBuffer.size(); i++) {
							currentPeak = std::max(currentPeak, mScratchBuffer[c][i]);
						}
					}
					mLimiterLastPeak = std::max(mLimiterLastPeak, currentPeak);
					mScratchBuffer.multiply(Sample(1.0) / mLimiterLastPeak);
				}
				mScratchBuffer.multiply(mMasterVolume);
				d.push(mScratchBuffer);
				mScratchBuffer.set(0);
				mTime += remaining;
				mTimeFract += step * remaining;
			}
			VAE_PROFILER_FRAME_MARK_END(profiler::audioFrame)
			VAE_PROFILER_FRAME_MARK()
			if (mConfig.updateInAudioThread) { update(); }
		}

		/**
		 * @brief Called from own audio thread, not the device
		 *
		 */
		void threadedProcess() {
			VEA_PROFILER_THREAD_NAME("Audio thread")
			while(mAudioThreadRunning) {
				process();	// Process one block in advance so there's no underrun
				std::unique_lock<std::mutex> l(mMutex);
				mAudioConsumed.wait(l);	// Wait until we got work
			}
		}

		/**
		 * @brief Called from audio device when it needs more audio.
		 * This will do synchronous processing.
		 * @param device
		 */
		void onBufferSwap(Device* device) {
			(void) device;
			process();
		}

		/**
		 * @brief Called from audio device when using seperate audio thread.
		 * This will only notify the adio thread to do work.
		 * @param device
		 */
		void onThreadedBufferSwap(Device* device) {
			(void) device;
			mAudioConsumed.notify_one();
		}

	public:

		Engine() = default;

		/**
		 * Don't allow any kind of move of copy of the object
		 */
		Engine(const Engine&) = delete;
		Engine(const Engine*) = delete;
		Engine(Engine&&) = delete;
		Engine& operator= (const Engine&) = delete;
		Engine& operator= (Engine&&) = delete;

		~Engine() {
			stop();
			unloadAllBanks();
			VAE_INFO("Engine destructed")
		}

		/** @name Engine Controls
		 * Main engine functionality
		 */
		///@{

		/**
		 * @brief Initializes the engine and does most of the upfront allocations. Run this before start !
		 * @details Everything will be allocated according to the provided config.
		 * Loading a Bank will still cause an allocation.
		 * If there are already banks loaded, they will be reloaded to have the correct samplerate.
		 * @see start
		 * @param config Optional config to setup the internals.
		 * @return Result
		 */
		Result _VAE_PUBLIC_API init(const EngineConfig& config = {}) {
			VAE_PROFILER_SCOPE_NAMED("Engine init")
			VAE_DEBUG("Initializing engine...")
			mConfig = config;
			mScratchBuffer.resize(StaticConfig::MaxBlock, StaticConfig::MaxChannels);
			mScratchBuffer.set(0);
			mScratchBuffer.sampleRate = mConfig.internalSampleRate;
			mVoiceManager.init(mConfig);
			mSpatialManager.init(mConfig.preAllocatedEmitters);
			mSpatialProcessor.init(mConfig.voices);
			mMixerProcessor.init();
			mBankManager.init(mConfig.rootPath, mConfig.internalSampleRate);
			VAE_DEBUG("Engine initialized")
			return Result::Success;
		}

		/**
		 * @brief Tries to open default device and start audio thread. Call this after start.
		 * @see init
		 * @return Result
		 */
		Result _VAE_PUBLIC_API start() {
			VEA_PROFILER_THREAD_NAME("Application Thread")
			VAE_PROFILER_SCOPE()
			{
				VAE_PROFILER_SCOPE_NAMED("Device Instance")
				Backend& backend = CurrentBackend::instance();
				mDevice = backend.createDevice(mConfig);
			}
			if (mConfig.processInBufferSwitch) {
				mDevice->setCallback(TKLB_DELEGATE(&Engine::onBufferSwap, *this));
				VAE_DEBUG("Mixing in buffer switch")
			} else {
				mDevice->setCallback(TKLB_DELEGATE(&Engine::onThreadedBufferSwap, *this));
				mAudioThreadRunning = true;
				VAE_PROFILER_SCOPE_NAMED("Start audio thread")
				mAudioThread = new Thread(&Engine::threadedProcess, this);
				VAE_DEBUG("Mixing in seperate thread")
			}
			{
				VAE_PROFILER_SCOPE_NAMED("Device Instance")
				return mDevice->openDevice() ? Result::Success : Result::DeviceError;
			}
		}

		/**
		 * @brief Stops processing and waits for audio thead to clean up
		 * @return Result
		 */
		Result _VAE_PUBLIC_API stop() {
			VAE_PROFILER_SCOPE()
			if (mAudioThreadRunning) {
				mAudioThreadRunning = false;
				mAudioConsumed.notify_one(); // make sure the audio thread knows it's time to go
				if(mAudioThread->joinable()) {
					mAudioThread->join();
					delete mAudioThread;
					VAE_DEBUG("Audio thread stopped")
				} else {
					VAE_ERROR("Can't join audio thread")
				}
			}
			if (mDevice != nullptr) {
				delete mDevice;
				mDevice = nullptr;
			}
			return Result::Success;
		}

		/**
		 * @brief Update function needs to be called regularly to handle outbound events and other housekeeping.
		 * @details If this isn't called regularly events might be lost and chained events not fired.
		 * When EngineConfig::updateInAudioThread is true, this doesn't need to be called manually.
		 * @see EngineConfig::updateInAudioThread
		 */
		void _VAE_PUBLIC_API update() {
			VAE_PROFILER_FRAME_MARK_START(profiler::audioFrame)
			VAE_PROFILER_SCOPE_NAMED("Engine Update")
			// Update emitters and start voices nearby
			mBankManager.lock();
			mSpatialManager.update(
				mVoiceManager,
				[&](EventHandle event, BankHandle bank, EmitterHandle emitter) {
					fireEvent(bank, event, emitter);
				}
			);

			// Handle finished voices and their events
			mVoiceManager.forEachFinishedVoice([&](Voice& v) {
				if (v.emitter != InvalidEmitterHandle) {
					mSpatialManager.getEmitter(v.emitter).autoplaying = false;
					// Make sure the event can be triggered again by that emitter
				}
				for (auto& i : mBankManager.get(v.bank).events[v.event].on_end) {
					if (i == InvalidEventHandle) { continue; }
					fireEvent(v.bank, i, v.emitter, 1.0, v.mixer);
				}
				return true;
			});
			mBankManager.unlock();
			VAE_PROFILER_FRAME_MARK_END(profiler::audioFrame)
		}

		/**
		 * @brief Main mechanism to start and stop sounds
		 * @see Event
		 * @param bankHandle bank id where the event is provided
		 * @param eventHandle id of the event
		 * @param emitterHandle handle of the emitter, needed for spatial audio or controlling the voice
		 * @param gain optional volume factor
		 * @param mixerHandle optional id of mixer channel sound will be routed to, this will override the one set in the event
		 * @param listenerHandle For which listener this event will be adible for, default to all
		 * @return Result
		 */
		Result _VAE_PUBLIC_API fireEvent(
			BankHandle bankHandle, EventHandle eventHandle,
			EmitterHandle emitterHandle,
			Sample gain = 1.0,
			MixerHandle mixerHandle = InvalidMixerHandle,
			ListenerHandle listenerHandle = AllListeners
		) {
			VAE_PROFILER_SCOPE()
			if (emitterHandle != InvalidEmitterHandle && !mSpatialManager.hasEmitter(emitterHandle)) {
				VAE_ERROR("No emitter %u registered", emitterHandle)
				return Result::InvalidEmitter;
			}

			if (!mBankManager.has(bankHandle)) {
				VAE_ERROR("Fired event %i on unloaded bank %i", eventHandle, bankHandle)
				return Result::InvalidBank;
			}

			auto& bank = mBankManager.get(bankHandle);

			if (bank.events.size() <= eventHandle) {
				VAE_WARN("Fired missing event %i on bank %i", eventHandle, bankHandle)
				return Result::ValidHandleRequired;
			}

			auto& event = bank.events[eventHandle];

			Result result;

			if (event.action == Event::Action::start) {
				if (event.source != InvalidSourceHandle) {
					VAE_DEBUG_EVENT("Event %i:%i starts source %i", eventHandle, bankHandle, event.source)
					// Has source attached
					if (event.spatial) {
						// Spatialized sounds will play for every listener
						result = mSpatialManager.forListeners(listenerHandle, [&](Listener& l) {
							return mVoiceManager.play(
								event, bankHandle, gain, emitterHandle, l.id, mixerHandle
							);
						});
					} else {
						// non spatialized sounds just once
						result = mVoiceManager.play(
							event, bankHandle, gain, emitterHandle, listenerHandle, mixerHandle
						);
					}
				}

				// Fire all other chained events
				for (auto& i : event.on_start) {
					if (i == InvalidEventHandle) { continue; }
					VAE_DEBUG_EVENT("Event %i:%i starts chained event %i", eventHandle, bankHandle, i)
					result = fireEvent(
						bankHandle, i, emitterHandle, gain,
						mixerHandle, listenerHandle
					);
				}

				if (result != Result::Success) {
					VAE_DEBUG("Event %i:%i failed to start voices", eventHandle, bankHandle)
					return result; // ! someting went wrong
				}
			}

			else if (event.action == Event::Action::random) {
				for (int index = rand() % event.on_start.size(); 0 <= index; index--) {
					auto& i = event.on_start[index];
					if (i == InvalidEventHandle) { continue; }
					VAE_DEBUG_EVENT("Event %i:%i starts random event %i", eventHandle, bankHandle, i)
					result = fireEvent(
						bankHandle, i, emitterHandle, gain,
						mixerHandle, listenerHandle
					);
					break;
				}
			}

			else if (event.action == Event::Action::stop) {
				// TODO test stopping
				if (event.source != InvalidSourceHandle) {
					VAE_DEBUG_EVENT("Event %i:%i stops source %i", eventHandle, bankHandle, event.source)
					mVoiceManager.stop(event.source, &Voice::source, emitterHandle);
				}
				for (auto& i : event.on_start) {
					if (i == InvalidEventHandle) { continue; }
					// kill every voice started from these events
					VAE_DEBUG_EVENT("Event %i:%i stops voices from event %i", eventHandle, bankHandle, i)
					mVoiceManager.stop(i, &Voice::event, emitterHandle);
				}
				if (event.mixer != Mixer::MasterMixerHandle) {
					// kill every voice in a mixer channel
					VAE_DEBUG_EVENT("Event %i:%i stops voices in mixer %i", eventHandle, bankHandle, event.mixer)
					mVoiceManager.stop(event.mixer, &Voice::mixer, emitterHandle);
				}
			}

			else if (event.action == Event::Action::emit) {
				VAE_DEBUG_EVENT("Event %i:%i emits event", eventHandle, bankHandle)
				if (mConfig.eventCallback != nullptr) {
					EventCallbackData data;
					constexpr int as = sizeof(data);
					data.context = mConfig.eventCallbackContext;
					data.bank = bankHandle;
					data.event = eventHandle;
					data.emitter = emitterHandle;
					mConfig.eventCallback(&data);
				}
			}

			return Result::Success;
		}

		/**
		 * @brief Works like fireEvent but with a global Event identifier
		 * @see fireEvent
		 * @param globalHandle The GlobalEventHandle combines both bank and event id
		 * @param emitterHandle optional handle of the emitter, needed for spatial audio
		 * @param gain optional volume factor
		 * @param mixerHandle id of mixer channel sound will be routed to, this will override the one set in the event
		 * @return Result
		 */
		Result _VAE_PUBLIC_API fireGlobalEvent(
			GlobalEventHandle globalHandle,
			EmitterHandle emitterHandle,
			Sample gain = 1.0,
			MixerHandle mixerHandle = InvalidMixerHandle,
			ListenerHandle listenerHandle = AllListeners
		) {
			BankHandle bankHandle;
			EventHandle eventHandle;
			splitGlobalEventHandle(globalHandle, bankHandle, eventHandle);
			return fireEvent(
				bankHandle, eventHandle, emitterHandle,
				gain, mixerHandle, listenerHandle
			);
		}

		/**
		 * @brief Get the number of currently playing Voices
		 */
		int _VAE_PUBLIC_API getActiveVoiceCount() const {
			return mVoiceManager.getActiveVoiceCount();
		}

		/**
		 * @brief Set the global output volume before the limiter.
		 * @details The engine can't clip, but if the output is too load
		 * the signal will be squashed in the limiter.
		 * @param volume 1.0 is the default, not interpolated for now
		 */
		void _VAE_PUBLIC_API setMasterVolume(Sample volume) {
			mMasterVolume = volume;
		}

		/**
		 * @brief Check if the compiled version matches
		 */
		bool _VAE_PUBLIC_API checkVersion(int major, int minor, int patch) {
			return
				VAE_VERSION_MAJOR == major &&
				VAE_VERSION_MINOR == minor &&
				VAE_VERSION_PATCH == patch;
		}

		///@}

#pragma region emitter

		/** @name Emitter manipulation
		 * Contains everything related to emitter creation and basic manipulation of voices started from them
		 */
		///@{

		/**
		 * @brief Creates an emitter and returns the handle
		 * @return EmitterHandle Random handle
		 */
		EmitterHandle _VAE_PUBLIC_API createEmitter() {
			return mSpatialManager.createEmitter();
		}

		/**
		 * @brief Emitter which triggers an event once a listener is close enough
		 *
		 * @param bank
		 * @param event
		 * @param maxDist
		 * @param locDir
		 * @param spread
		 * @return EmitterHandle Handle like a normal emitter
		 */
		EmitterHandle _VAE_PUBLIC_API createAutoEmitter(
			BankHandle bank, EventHandle event, float maxDist,
			const LocationDirection& locDir, float spread
		) {
			return mSpatialManager.createAutoEmitter(bank, event, maxDist, locDir, spread);
		}

		/**
		 * @brief Adds an emitter with a custom handle, can be an internal ID for example
		 * @details migt be desireable to make EmitterHandle the same size as a pointer
		 * so this can simply be the pointer of the entity that is associated with it.
		 * @param h
		 * @return Result
		 */
		Result _VAE_PUBLIC_API addEmitter(EmitterHandle h) {
			return mSpatialManager.addEmitter(h);
		}

		/**
		 * @brief Unregister a emiter an kill all its voices
		 * @param h
		 * @return Result
		 */
		Result _VAE_PUBLIC_API removeEmitter(EmitterHandle h) {
			mVoiceManager.stop(h, &Voice::emitter);
			return mSpatialManager.removeEmitter(h);
		}

		/**
		 * @brief Set the Emitter position, orientation and spread
		 * @param emitter The emitter
		 * @param locDir The desired location
		 * @param spread The width of the panning (if it's spatial and not HRTF)
		 * @return Result
		 */
		Result _VAE_PUBLIC_API setEmitter(
			EmitterHandle emitter, const LocationDirection& locDir, float spread
		) {
			return mSpatialManager.setEmitter(emitter, locDir, spread);
		}

		/**
		 * @brief Stop all voices from emitter.
		 * @param emitter
		 * @return Result
		 */
		Result _VAE_PUBLIC_API stopEmitter(EmitterHandle emitter) {
			return mVoiceManager.stop(emitter, &Voice::emitter);
		}

		/**
		 * @brief Sets the volume of all active voices with this emitter
		 * @param emitter
		 * @param gain
		 */
		void _VAE_PUBLIC_API setVolume(EmitterHandle emitter, Sample gain) {
			mVoiceManager.setVoiceProperty(emitter, &Voice::gain, gain);
		}

		/**
		 * @brief Set the current time of all voices with the emitter.
		 * @param emitter
		 * @param time Time in samples
		 */
		void _VAE_PUBLIC_API seek(EmitterHandle emitter, Size time) {
			mVoiceManager.setVoiceProperty(emitter, &Voice::time, SampleIndex(time));
		}

		/**
		 * @brief Set the playback speed
		 * @param emitter
		 * @param speed 1.0 is the default speed, pitch will be affected as well.
		 */
		void _VAE_PUBLIC_API setSpeed(EmitterHandle emitter, float speed) {
			mVoiceManager.setVoiceProperty(emitter, &VoiceFilter::speed, speed);
		}

		/**
		 * @brief Simple lowpass filter for the voices
		 * @param emitter
		 * @param cutoff 0-1. 0 doesn't filter, 1 filter the wholespektrum
		 */
		void _VAE_PUBLIC_API setLowpass(EmitterHandle emitter, float cutoff) {
			mVoiceManager.setVoiceProperty(emitter, &VoiceFilter::lowpass, cutoff);
		}

		/**
		 * @brief Simple highpass filter for the voices
		 * @param emitter
		 * @param cutoff 0-1. 0 doesn't filter, 1 filter the wholespektrum
		 */
		void _VAE_PUBLIC_API setHighpass(EmitterHandle emitter, float cutoff) {
			mVoiceManager.setVoiceProperty(emitter, &VoiceFilter::highpass, cutoff);
		}

		///@}
#pragma endregion emitter

		/**
		 * @brief Set the Mixer Volume
		 *
		 * @param bank
		 * @param mixer
		 * @param volume
		 * @return Result
		 */
		Result setMixerVolume(BankHandle bank, MixerHandle mixer, Sample volume) {
			mBankManager.get(bank).mixers[mixer].gain = volume;
			return Result::Success;
		}

		/**
		 * @brief Bypass a effect in a mixer
		 *
		 * @param bank
		 * @param mixer
		 * @param index See setMixerEffectParameter
		 * @param mute
		 * @return Result
		 */
		Result muteMixerEffect(BankHandle bank, MixerHandle mixer, Size index, bool mute) {
			mBankManager.get(bank).mixers[mixer].effects[index].bypassed = mute;
		}

		/**
		 * @brief Set the Mixer Effect Parameter
		 *
		 * @param bank
		 * @param mixer
		 * @param index which effects slot out of StaticConfig::MaxMixerEffects
		 * @param param	which param by index out of StaticConfig::MaxEffectsParameter
		 * @param value value of the parameter from 0 to 1
		 * @return Result
		 */
		Result setMixerEffectParameter(BankHandle bank, MixerHandle mixer, Size index, Size param, Sample value) {
			// TODO this is garbage but needs a event queue anyways
			mBankManager.get(bank).mixers[mixer].effects[index].parameters[param].value = value;
			return Result::Success;
		}
#pragma region mixer



#pragma endregion emitter

#pragma region listener

		/**
		 * @brief Create a Listener object
		 * @details TODO make listener 0 implicit
		 * @return ListenerHandle
		 */
		ListenerHandle _VAE_PUBLIC_API createListener() {
			return mSpatialManager.createListener();
		}

		/**
		 * @brief Unregister listener
		 * @param listener
		 */
		Result _VAE_PUBLIC_API removeListener(ListenerHandle listener) {
			return mSpatialManager.removeListener(listener);
		}

		/**
		 * @brief Set the position of a listener
		 * @param listener
		 * @return Result
		 */
		Result _VAE_PUBLIC_API setListener(ListenerHandle listener, const LocationOrientation& locOr) {
			return mSpatialManager.setListener(listener, locOr);
		}

		Result _VAE_PUBLIC_API loadHRTF(const char* path) {
			return mSpatialProcessor.loadHRTF(
				path, mConfig.rootPath,
				mConfig.internalSampleRate
			);
		}

#pragma endregion listsner


#pragma region bank_handling

		/** @name Ressource Management
		 * Everything related to Bank and other ressource loading/unloading
		 */
		///@{
		/**
		 * @brief Load bank from filesystem.
		 * @details This operation might take a little time but won't lock the audio thread
		 * until the bank is inserted. This should be safe to do at any time.
		 * @param path
		 * @return Result
		 */
		Result _VAE_PUBLIC_API loadBank(const char* path) {
			return mBankManager.load(path, mConfig.rootPath, mConfig.internalSampleRate);
		}

		/**
		 * @brief Load bank from memory.
		 * @see loadBank
		 * @param bank Moved and now owned by the engine
		 * @return Result
		 */
		Result loadBank(Bank& bank) {
			return mBankManager.load(bank, mConfig.internalSampleRate);
		}

		/**
		 * @brief Add or replace a source in a bank.
		 * @param bankHandle
		 * @param source Moved and now owned by bank
		 * @return Result
		 */
		Result addSource(BankHandle bankHandle, Source& source) {
			return mBankManager.addSource(bankHandle, source, mConfig.internalSampleRate);
		}

		/**
		 * @brief Add or replace event in a bank
		 * Locks audio thread
		 * @param bankHandle
		 * @param event Moved and now owned by bank
		 * @return Result
		 */
		Result addEvent(BankHandle bankHandle, Event& event) {
			return mBankManager.addEvent(bankHandle, event);
		}

		Result addMixer(BankHandle bankHandle, Mixer& mixer) {
			return mBankManager.addMixer(bankHandle, mixer);
		}

		/**
		 * @brief Unload bank from handle.
		 * Locks audio thread and stops all voices from that bank.
		 * @param bankHandle
		 * @return Result
		 */
		Result _VAE_PUBLIC_API unloadBankFromId(BankHandle bankHandle) {
			VAE_INFO("Start Unload bank %i", bankHandle)
			mVoiceManager.stop(bankHandle, &Voice::bank);
			return mBankManager.unloadFromId(bankHandle);
		}

		/**
		 * @brief Unload every bank and data associated with it
		 */
		void _VAE_PUBLIC_API unloadAllBanks() {
			VAE_INFO("Start unloading all banks")
			mBankManager.unloadAll();
		}

		///@}
#pragma endregion bank_handling


	}; // Engine class
	constexpr int _VAE_ENGINE_SIZE = sizeof(Engine);
} } // namespace vae::core

#endif // _VAE_ENGINE
