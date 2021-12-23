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

#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "../../external/tklb/src/util/TMath.hpp"
#include "vae/vae.hpp"
#include "vae_logger.hpp"

#include <cstring>				// strcmp
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
		using Mutex = tklb::SpinLock;
		using Lock = tklb::LockGuard<Mutex>;
		using CurrentBackend = BackendRtAudio;

		using Thread = std::thread;
		using Semaphore = std::condition_variable;

		EngineConfig mConfig;			///< Config object provided at initlalization

		VoiceManger mVoiceManager;		///< Holds and handle voices
		SpatialManager mSpatialManager;	///< Holds and manages spatial emitters
		BankManager mBankManager;		///< Holds and manages banks

		Processor mProcessor;
		MixerProcessor mMixerProcessor;
		SpatialProcessor mSpatialProcessor;


		Device* mDevice = nullptr;		///< Output device
		AudioBuffer mScratchBuffer;		///< used to combine the signal from all banks and push it to the device
		SampleIndex mTime = 0;			///< Global engine time in samples
		Time mTimeFract = 0;			///< Global engine time in seconds
		Sample mLimiterLastPeak = 1.0;	///< Master limiter last peak
		Sample mMasterVolume = 1.0;		///< Master Colume applied after limiting

		Thread* mAudioThread;			///< Thread processing voices and mixers
		Semaphore* mAudioConsumed;		///< Notifies the audio thread when more audio is needed
		Mutex mMutex;					///< Mutex to lock AudioThread and bank operations
		bool mAudioThreadRunning = false;

		/**
		 * @brief Main processing function
		 * Called either from onBufferSwap or threadedProcess
		 */
		void process() {
			VAE_PROFILER_FRAME_MARK_START(_VAE_PROFILER_AUDIO)
			auto& d = *mDevice;
			auto sampleRate = d.getSampleRate();

			const Time step = 1.0 / Time(sampleRate);

			{
				Lock l(mMutex);
				// process until output is full
				while (true) {
					// ! this is an estimate when resampling
					auto remaining = d.canPush();

					static_assert(32 <= Config::MaxBlock, "MaxBlock needs to be larger");

					if (remaining < 32) {
						break; // ! Don't even bother with small blocks, we'll get em next time
					}

					// clamp to max processable size
					remaining = std::min(remaining, Config::MaxBlock);

					mScratchBuffer.setValidSize(remaining);

					// TODO PERF VAE banks could be processed in parallel
					// however each bank needs to get a temporary own copy of the voice
					// or else this will be false sharing city
					mBankManager.forEach([&](Bank& i) {
						mProcessor.mix(mVoiceManager, i, remaining, sampleRate);
						mSpatialProcessor.mix(
							mVoiceManager, i, mSpatialManager, remaining, sampleRate
						);
						mMixerProcessor.mix(mVoiceManager, i, remaining);
						auto& bankMaster = i.mixers[Mixer::MasterMixerHandle].buffer;
						mScratchBuffer.add(bankMaster);
						bankMaster.set(0);
					});

					{
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
			}
			VAE_PROFILER_FRAME_MARK_END(_VAE_PROFILER_AUDIO)
			if (mConfig.updateInAudioThread) { update(); }
		}

		void threadedProcess() {
			while(mAudioThreadRunning) {
				process();
				// TODO wait for device to notify
				std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2));
				// std::unique_lock<std::mutex> lck(mutex_);
				// mAudioConsumed.wait(lck);
			}
		}

		/**
		 * @brief Called from audio devices when they are about to
		 * swap buffers. This will do sync audio processing
		 * @param device
		 */
		void onBufferSwap(Device* device) {
			process();
		}

		/**
		 * @brief Called from audio devices when they are about to
		 * swap buffers. This will only notify the audio thread to wake up.
		 * @param device
		 */
		void onThreadedBufferSwap(Device* device) {
			// mAudioConsumed->notify_one();
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

		/**
		 * @brief Initialized the engine and does most of the upfront allocations.
		 * @details Everything will be allocated according to the provided config.
		 * Loading a Bank will still cause an allocation.
		 * If there are already banks loaded, they will be reloaded to have the correct samplerate.
		 * @param config Optional config to setup the internals.
		 * @return Result
		 */
		Result _VAE_PUBLIC_API init(const EngineConfig& config = {}) {
			VAE_PROFILER_SCOPE
			VAE_DEBUG("Initializing engine...")
			mConfig = config;
			mScratchBuffer.resize(Config::MaxBlock, Config::MaxChannels);
			mScratchBuffer.set(0);
			mScratchBuffer.sampleRate = mConfig.internalSampleRate;
			mVoiceManager.init(mConfig);
			mSpatialManager.init(mConfig.preAllocatedEmitters);
			mSpatialProcessor.init(mConfig.voices);
			mBankManager.init(mConfig.rootPath, mConfig.internalSampleRate);
			VAE_DEBUG("Engine initialized")
			return Result::Success;
		}

		~Engine() {
			stop();
			unloadAllBanks();
			VAE_INFO("Engine destructed")
		}

		/**
		 * @brief Tries to open default device and start audio thread.
		 * @return Result
		 */
		Result _VAE_PUBLIC_API start() {
			VAE_PROFILER_SCOPE
			Backend& backend = CurrentBackend::instance();
			mDevice = backend.createDevice(mConfig);
			if (mConfig.processInBufferSwitch) {
				mDevice->setCallback(TKLB_DELEGATE(&Engine::onBufferSwap, *this));
				VAE_DEBUG("Mixing in buffer switch")
			} else {
				mDevice->setCallback(TKLB_DELEGATE(&Engine::onThreadedBufferSwap, *this));
				mAudioThreadRunning = true;
				mAudioThread = new Thread(&Engine::threadedProcess, this);
				VAE_DEBUG("Mixing in seperate thread")
			}
			return mDevice->openDevice() ? Result::Success : Result::DeviceError;
		}

		/**
		 * @brief Stops processing and waits for audio thead to clean up
		 * @return Result
		 */
		Result _VAE_PUBLIC_API stop() {
			VAE_PROFILER_SCOPE
			Lock l(mMutex);
			mBankManager.lock();
			if (mAudioThreadRunning) {
				mAudioThreadRunning = false;
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
			mBankManager.unlock();
			return Result::Success;
		}

		/**
		 * @brief Update function needs to be called regularly to handle outbound events.
		 * If this isn't called regularly events might be lost.
		 */
		void _VAE_PUBLIC_API update() {
			VAE_PROFILER_FRAME_MARK_START(_VAE_PROFILER_UPDATE)
			// Update emitters and start voices nearby
			mBankManager.lock();
			mSpatialManager.update(mVoiceManager, mBankManager);

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
			VAE_PROFILER_FRAME_MARK_END(_VAE_PROFILER_UPDATE)

		}

		/**
		 * @brief Main mechanism to start and stop sounds
		 *
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
			float gain = 1.0,
			MixerHandle mixerHandle = InvalidMixerHandle,
			ListenerHandle listenerHandle = AllListeners
		) {
			VAE_PROFILER_SCOPE
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

			if (event.start) {
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
				if (event.random) {
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
				} else {
					// Fire all other chained events
					for (auto& i : event.on_start) {
						if (i == InvalidEventHandle) { continue; }
						VAE_DEBUG_EVENT("Event %i:%i starts chained event %i", eventHandle, bankHandle, i)
						result = fireEvent(
							bankHandle, i, emitterHandle, gain,
							mixerHandle, listenerHandle
						);
					}
				}
				if (result != Result::Success) {
					VAE_DEBUG("Event %i:%i failed to start voices. Result %i", eventHandle, bankHandle, result)
					return result; // ! someting went wrong
				}
			}

			if (event.stop) {
				// TODO test stopping
				if (event.source != InvalidSourceHandle) {
					VAE_DEBUG_EVENT("Event %i:%i stops source %i", eventHandle, bankHandle, event.source)
					mVoiceManager.stopFromSource(event.source, emitterHandle);
				}
				for (auto& i : event.on_start) {
					if (i == InvalidEventHandle) { continue; }
					// kill every voice started from these events
					VAE_DEBUG_EVENT("Event %i:%i stops voices from event %i", eventHandle, bankHandle, i)
					mVoiceManager.stopFromEvent(i, emitterHandle);
				}
				if (event.mixer != Mixer::MasterMixerHandle) {
					// kill every voice in a mixer channel
					VAE_DEBUG_EVENT("Event %i:%i stops voices in mixer %i", eventHandle, bankHandle, event.mixer)
					mVoiceManager.stopFromMixer(event.mixer, emitterHandle);
				}
			}

			if (event.emit) {
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
		 *
		 * @param globalHandle The GlobalEventHandle combines both bank and event id
		 * @param emitterHandle optional handle of the emitter, needed for spatial audio
		 * @param gain optional volume factor
		 * @param mixerHandle id of mixer channel sound will be routed to, this will override the one set in the event
		 * @see fireEvent
		 * @return Result
		 */
		Result _VAE_PUBLIC_API fireGlobalEvent(
			GlobalEventHandle globalHandle,
			EmitterHandle emitterHandle,
			float gain = 1.0,
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


		Result _VAE_PUBLIC_API stopEmitter(EmitterHandle emitter) {
			return mVoiceManager.stopEmitter(emitter);
		}

		/**
		 * @brief Get the number of currently playing Voices
		 */
		int _VAE_PUBLIC_API getActiveVoiceCount() const {
			return mVoiceManager.getActiveVoiceCount();
		}

		void _VAE_PUBLIC_API setMasterVolume(float volume) {
			mMasterVolume = volume;
		}


#pragma region emitter

		EmitterHandle _VAE_PUBLIC_API createEmitter() {
			return mSpatialManager.createEmitter();
		}

		EmitterHandle _VAE_PUBLIC_API createAutoEmitter(
			BankHandle bank, EventHandle event, float maxDist,
			const LocationDirection& locDir, float spread
		) {
			return mSpatialManager.createAutoEmitter(bank, event, maxDist, locDir, spread);
		}

		Result _VAE_PUBLIC_API addEmitter(EmitterHandle h) {
			return mSpatialManager.addEmitter(h);
		}

		Result _VAE_PUBLIC_API removeEmitter(EmitterHandle h) {
			mVoiceManager.stopEmitter(h);
			return mSpatialManager.removeEmitter(h);
		}

		Result _VAE_PUBLIC_API setEmitter(
			EmitterHandle emitter, const LocationDirection& locDir,
			float spread
		) {
			return mSpatialManager.setEmitter(emitter, locDir, spread);
		}

		// Result stopEmitter(EmitterHandle emitter) {
		// 	return mVoiceManager.stopEmitter(emitter);
		// }

		ListenerHandle _VAE_PUBLIC_API createListener() {
			return mSpatialManager.createListener();
		}

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



#pragma endregion emitter

#pragma region bank_handling
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
		 * @brief Unload bank from path
		 * Locks audio thread
		 * @param path
		 * @return Result
		 */
		Result _VAE_PUBLIC_API unloadBankFromPath(const char* path) {
			return mBankManager.unloadFromPath(path);
		}

		/**
		 * @brief Unload bank from handle
		 * Locks audio thread
		 * @param bankHandle
		 * @return Result
		 */
		Result _VAE_PUBLIC_API unloadBankFromId(BankHandle bankHandle) {
			return mBankManager.unloadFromId(bankHandle);
		}

		/**
		 * @brief Unload every bank and data associated with it
		 */
		void _VAE_PUBLIC_API unloadAllBanks() {
			mBankManager.unloadAll();
		}
#pragma endregion bank_handling

		/**
		 * @brief Check if the compiled version matches
		 */
		bool _VAE_PUBLIC_API checkVersion(int major, int minor, int patch) {
			return
				VAE_VERSION_MAJOR == major &&
				VAE_VERSION_MINOR == minor &&
				VAE_VERSION_PATCH == patch;
		}

	}; // Engine class
	constexpr int _VAE_ENGINE_SIZE = sizeof(Engine);
} } // namespace vae::core

#endif // _VAE_ENGINE
