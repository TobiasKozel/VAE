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
#include "./vae_event_manager.hpp"
#include "./processor/vae_processor.hpp"
#include "./processor/vae_spatial_processor.hpp"
#include "./processor/vae_mixer_processor.hpp"
#include "./vae_bank_manager.hpp"

#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "../../external/tklb/src/util/TMath.hpp"

#include <cstring>				// strcmp
#include <thread>				// mAudioThread
#include <condition_variable>	// mAudioConsumed

/**
 * @brief Marks a function for export to the generated EnginePimpl class
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

		const EngineConfig mConfig;		// Config object provided at construction

		VoiceManger mVoiceManager;		// Holds and handle voices
		SpatialManager mSpatialManager;	// Holds and manages spatial emitters
		BankManager mBankManager;		// Holds and manages banks
		EventManager mEventManager;

		Processor mProcessor;
		MixerProcessor mMixerProcessor;
		SpatialProcessor mSpatialProcessor;


		Device* mDevice = nullptr;		// Output device
		AudioBuffer mScratchBuffer;		// used to combine the signal from all banks and push it to the device
		SampleIndex mTime = 0;			// Global engine time in samples
		Time mTimeFract = 0;			// Global engine time in seconds
		Sample mLimiterLastPeak = 1.0;	// Master limiter last peak

		Thread* mAudioThread;			// Thread processing voices and mixers
		Semaphore* mAudioConsumed;		// Notifies the audio thread when more audio is needed
		Mutex mMutex;					// Mutex to lock AudioThread and bank operations
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
			mAudioConsumed->notify_one();
		}

		void postContructor() {
			VAE_PROFILER_SCOPE
			mScratchBuffer.resize(Config::MaxBlock, Config::MaxChannels);
			mScratchBuffer.set(0);
			mScratchBuffer.sampleRate = mConfig.internalSampleRate;
			VAE_DEBUG("Engine constructed")
		}

	public:
		_VAE_PUBLIC_API Engine(const EngineConfig& config) :
			mConfig(config),
		 	mVoiceManager(mConfig.voices, mConfig.virtualVoices),
			mSpatialManager(mConfig.preAllocatedEmitters)
		{
			postContructor();
		}

		_VAE_PUBLIC_API Engine() :
			mConfig({}),
		 	mVoiceManager(mConfig.voices, mConfig.virtualVoices),
			mSpatialManager(mConfig.preAllocatedEmitters)
		{
			postContructor();
		}

		~Engine() {
			unloadAllBanks();
			stop();
			VAE_INFO("Engine destructed")
		}


		/**
		 * Don't allow any kind of move of copy of the object
		 */
		Engine(const Engine&) = delete;
		Engine(const Engine*) = delete;
		Engine(Engine&&) = delete;
		Engine& operator= (const Engine&) = delete;
		Engine& operator= (Engine&&) = delete;

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
			return Result::Success;
		}

		/**
		 * @brief Update function needs to be called regularly to handle outbound events.
		 * If this isn't called regularly events might be lost.
		 */
		void _VAE_PUBLIC_API update() {
			VAE_PROFILER_FRAME_MARK_START(_VAE_PROFILER_UPDATE)
			// Update emitters and start voices nearby
			mSpatialManager.update(mVoiceManager, mBankManager);

			// Handle finished voices and their events
			mVoiceManager.forEachFinishedVoice([&](Voice& v) {
				if (v.emitter != InvalidEmitterHandle) {
					mSpatialManager.getEmitter(v.emitter).autoplaying = false;
					// Make sure the event can be triggered again by that emitter
				}
				for (auto& i : mBankManager.get(v.bank).events[v.event].on_end) {
					if (i == InvalidEventHandle) { continue; }
					fireEvent(v.bank, i, v.emitter, v.mixer);
				}
				return true;
			});
			VAE_PROFILER_FRAME_MARK_END(_VAE_PROFILER_UPDATE)

		}

		/**
		 * @brief Main mechanism to start and stop sounds
		 *
		 * @param bankHandle bank id where the event is provided
		 * @param eventHandle id of the event
		 * @param emitterHandle optional handle of the emitter, needed for spatial audio
		 * @param mixerHandle id of mixer channel sound will be routed to, this will override the one set in the event
		 * @return Result
		 */
		Result _VAE_PUBLIC_API fireEvent(
			BankHandle bank, EventHandle eventHandle,
			EmitterHandle emitterHandle = InvalidEmitterHandle,
			MixerHandle mixerHandle = InvalidMixerHandle
		) {
			if (emitterHandle != InvalidEmitterHandle && !mSpatialManager.hasEmitter(emitterHandle)) {
				VAE_ERROR("No emitter %u registered, voice won't be audible.", emitterHandle)
			}
			return mEventManager.fireEvent(
				bank, eventHandle,
				emitterHandle, mixerHandle,
				mBankManager, mVoiceManager, mConfig
			);
		}

		/**
		 * @brief Works like fireEvent but with a global Event identifier
		 *
		 * @param globalHandle The GlobalEventHandle combines both bank and event id
		 * @param emitterHandle optional handle of the emitter, needed for spatial audio
		 * @param mixerHandle id of mixer channel sound will be routed to, this will override the one set in the event
		 * @see fireEvent
		 * @return Result
		 */
		Result _VAE_PUBLIC_API fireGlobalEvent(
			GlobalEventHandle globalHandle,
			EmitterHandle emitterHandle = InvalidEmitterHandle,
			MixerHandle mixerHandle = InvalidMixerHandle
		) {
			BankHandle bankHandle;
			EventHandle eventHandle;
			splitGlobalEventHandle(globalHandle, bankHandle, eventHandle);
			return fireEvent(
				bankHandle, eventHandle, emitterHandle, mixerHandle
			);
		}


		Result _VAE_PUBLIC_API stopEmitter(EmitterHandle emitter) {
			return mVoiceManager.stopEmitter(emitter);
		}

		/**
		 * @brief Get the number of currently playing Voices
		 */
		int getActiveVoiceCount() const {
			return mVoiceManager.getActiveVoiceCount();
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
		 * @brief Load bank from filesystem
		 * Locks audio thread
		 * @param path
		 * @return Result
		 */
		Result _VAE_PUBLIC_API loadBank(const char* path) {
			return mBankManager.load(path, mConfig.rootPath, mConfig.internalSampleRate);
		}

		/**
		 * @brief Load bank from memory
		 * Locks audio thread
		 * @param bank Moved and now owned by the engine
		 * @return Result
		 */
		Result loadBank(Bank& bank) {
			return mBankManager.load(bank, mConfig.internalSampleRate);
		}

		/**
		 * @brief Add or replace a source in a bank
		 * Locks audio thread
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
