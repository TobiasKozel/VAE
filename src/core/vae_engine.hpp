#ifndef _VAE_ENGINE
#define _VAE_ENGINE


#include "./vae_util.hpp"
#include "../wrapped/vae_profiler.hpp"

#include "./vae_types.hpp"

#include "./device/vae_rtaudio.hpp"
#include "./device/vae_portaudio.hpp"

// #include "./vae_emitter.hpp"
#include "./vae_bus_events.hpp"
#include "./voice/vae_voice_manager.hpp"
#include "./fs/vae_bank_loader.hpp"
#include "./processor/vae_processor.hpp"
#include "./processor/vae_mixer_processor.hpp"
#include "./vae_util.hpp"

#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "../../external/tklb/src/util/TMath.hpp"

#include <cstring>	// strcmp
#include <vector>	// mBanks
#include <thread>	// mAudioThread


namespace vae { namespace core {

	class Engine {
		using Mutex = tklb::SpinLock;
		using Lock = tklb::LockGuard<Mutex>;
		using CurrentBackend = BackendRtAudio;
		using Thread = std::thread;

		/**
		 * Main messaging instance, most objects hold a
		 * reference to this instead of the engine itself
		 */
		// EventBus mBus;

		EngineConfig mConfig;			// Config provided at construction
		std::vector<Bank> mBanks;		// All the currently loaded banks
		VoiceManger mVoiceManager;		// Stores and handle voices
		Device* mDevice = nullptr;		// Output device
		SampleIndex mTime = 0;			// Global engine time in samples
		Time mTimeFract = 0;			// Global engine time in seconds
		Mutex mMutex;					// Mutex to lock AudioThread and bank operations
		Thread mAudioThread;			// Thread processing voices and mixers
		AudioBuffer mScratchBuffer;		//
		bool mAudioThreadRunning = false;
		// Listener mListener;

		Engine(const Engine&) = delete;
		Engine(const Engine*) = delete;
		Engine(Engine&&) = delete;
		Engine& operator= (const Engine&) = delete;
		Engine& operator= (Engine&&) = delete;

	public:
		Engine(
			EngineConfig& config
		) : mConfig(config), mVoiceManager(config) {
			VAE_DEBUG("Engine constructed")
		}

		~Engine() { stop(); }

		Result init() {
			Backend& backend = CurrentBackend::instance();
			mDevice = backend.createDevice(mConfig);
			mDevice->setCallback(TKLB_DELEGATE(&Engine::process, *this));
			mDevice->openDevice();

			mAudioThreadRunning = true;
			mScratchBuffer.resize(Config::MaxBlock, Config::MaxChannels);
			mScratchBuffer.sampleRate = mConfig.preferredSampleRate;
			// mAudioThread = Thread([&]() {
			// 	while(mAudioThreadRunning) {
			// 		// TODO wait for notification from device
			// 		process(mDevice);
			// 	}
			// });
			// VAE_DEBUG("Audio thread running")
			return Result::Success;
		}

		/**
		 * @brief Stops processing and waits for audio thead to clean up
		 * @return Result
		 */
		Result stop() {
			mAudioThreadRunning = false;
			if(mAudioThread.joinable()) {
				mAudioThread.join();
			}
			TKLB_DELETE(mDevice);
			VAE_INFO("Engine stopped")
			return Result::Success;
		}

		/**
		 * @brief Main processing function
		 *
		 * @param device Output device
		 */
		void process(Device* device) {
			auto& d = *device;

			auto sampleRate = d.getSampleRate();
			const Time step = 1.0 / Time(sampleRate);
			Lock l(mMutex);

			// process until output is full
			while (true) {
				// split larger chunks
				auto remaining = std::min(d.canPush(), Config::MaxBlock);
				if (remaining == 0) { break; }
				mScratchBuffer.setValidSize(remaining);
				// TODO PERF VAE banks could be processed in parellel
				for (auto& i : mBanks) {
					Processor::mix(mVoiceManager, i, remaining, sampleRate);
					MixerProcessor::mix(mVoiceManager, i, remaining);
					auto& bankMaster = i.mixers[Mixer::MasterMixerHandle].buffer;
					mScratchBuffer.add(bankMaster);
					bankMaster.set(0);
				}
				d.push(mScratchBuffer);
				mScratchBuffer.set(0);
				mTime += remaining;
				mTimeFract += step * remaining;
			}
			VAE_PROFILER_FRAME_MARK
		}

		/**
		 * @brief Update function needs to be called regularly to handle outbound events.
		 * If this isn't called regularly events might be lost.
		 */
		void update() {
			for (auto& v : mVoiceManager.voicesFinished) {
				if (v.source == InvalidHandle) { continue; }
				for (auto& i : mBanks[v.bank].events[v.event].on_end) {
					fireEvent(v.bank, i, v.emitter, v.mixer);
				}
				v.source = InvalidHandle; // now the finished voice is handled
			}
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
		Result fireEvent(
			BankHandle bankHandle, EventHandle eventHandle,
			EmitterHandle emitterHandle = InvalidHandle,
			MixerHandle mixerHandle = InvalidMixerHandle
		) {
			auto& bank = mBanks[bankHandle];
			auto& event = bank.events[eventHandle];
			Result result;

			switch (event.type) {
			case Event::EventType::start:
				if (event.source != InvalidHandle) {
					// Has source attached
					result = mVoiceManager.play(event, bankHandle, emitterHandle, mixerHandle);
					if (result != Result::Success) {
						// Failed to play for some reason
						return result;
					}
				}

				// Fire all other chained events
				for (auto& i : event.on_start) {
					fireEvent(bankHandle, i, emitterHandle, mixerHandle);
				}
				break;
			case Event::EventType::stop:
				// TODO test stopping
				mVoiceManager.stopFromSource(event.source, emitterHandle);
				for (auto& i : event.on_start) {
					// kill every voice started from these events
					mVoiceManager.stopFromEvent(i, emitterHandle);
				}
				if (event.mixer != Mixer::MasterMixerHandle) {
					// kill every voice in a mixer channel
					mVoiceManager.stopFromMixer(event.mixer, emitterHandle);
				}
				break;
			case Event::EventType::emit:
				if (mConfig.eventCallback == nullptr) { break; }
				EventCallbackData data;
				constexpr int as = sizeof(data);
				data.name = event.name.c_str();
				data.payload = mConfig.eventCallbackPayload;
				data.bank = bankHandle;
				data.event = eventHandle;
				data.emitter = emitterHandle;
				mConfig.eventCallback(&data);
				break;
			}

			return Result::Success;
		}

#pragma region bank_handling
		/**
		 * @brief Load bank from filesystem
		 * Locks audio thread
		 * @param path
		 * @return Result
		 */
		Result loadBank(const char* path) {
			VAE_INFO("Loading bank from file %s", path)
			Bank bank;
			auto result = BankLoader::load(path, bank);
			if (result != Result::Success) {
				VAE_WARN("Failed to load bank from file %s with error %i", path, result)
				return result;
			}
			return loadBank(bank);
		}

		/**
		 * @brief Load bank from memory
		 * Locks audio thread
		 * @param bank Moved and now owned by the engine
		 * @return Result
		 */
		Result loadBank(Bank& bank) {
			// TODO init mixer effects

			if (bank.mixers.empty()) {
				// Ensure there's a master channel per bank
				bank.mixers.resize(1);
				bank.mixers[0].id = 0;
				bank.mixers[0].name = "Bank Master";
			}

			for (auto& m : bank.mixers) {
				m.buffer.resize(Config::MaxBlock, Config::MaxChannels);
			}

			{
				Lock l(mMutex);
				if (mBanks.size() < bank.id + 1) {
					mBanks.resize(bank.id + 1);
				}
				mBanks[bank.id] = std::move(bank);
			}
			VAE_INFO("Bank %s loaded.", mBanks[bank.id].name.c_str())
			return Result::Success;
		}

		/**
		 * @brief Add or replace a source in a bank
		 * Locks audio thread
		 * @param bankHandle
		 * @param source Moved and now owned by bank
		 * @return Result
		 */
		Result addSource(BankHandle bankHandle, Source& source) {
			auto& bank = mBanks[bankHandle];
			Lock l(mMutex);
			if (bank.sources.size() <= source.id) {
				bank.sources.resize(source.id + 1);
			}
			bank.sources[source.id] = std::move(source);
			return Result::Success;
		}

		/**
		 * @brief Add or replace event in a bank
		 * Locks audio thread
		 * @param bankHandle
		 * @param event Moved and now owned by bank
		 * @return Result
		 */
		Result addEvent(BankHandle bankHandle, Event& event) {
			auto& bank = mBanks[bankHandle];
			Lock l(mMutex);
			if (bank.events.size() <= event.id) {
				bank.events.resize(event.id + 1);
			}
			bank.events[event.id] = std::move(event);
			return Result::Success;
		}

		Result addMixer(BankHandle bankHandle, Mixer& mixer) {
			// TODO init mixer effects
			mixer.buffer.resize(Config::MaxBlock, Config::MaxChannels);
			Lock l(mMutex);
			auto& bank = mBanks[bankHandle];
			if (bank.mixers.size() <= mixer.id) {
				bank.mixers.resize(mixer.id + 1);
			}
			bank.mixers[mixer.id] = std::move(mixer);
			return Result::Success;
		}

		/**
		 * @brief Unload bank from path
		 * Locks audio thread
		 * @param path
		 * @return Result
		 */
		Result unloadBankFromPath(const char* path) {
			Lock l(mMutex);
			for (auto& i : mBanks) {
				if (strcmp(path, i.path.c_str()) == 0) {
					i = { }; // should free all the memory
					VAE_DEBUG("Bank %s unloaded.", path)
					return Result::Success;
				}
			}
			VAE_WARN("Could not unload Bank %s", path)
			return Result::ElementNotFound;
		}

		/**
		 * @brief Unload bank from handle
		 * Locks audio thread
		 * @param bankHandle
		 * @return Result
		 */
		Result unloadBankFromId(BankHandle bankHandle) {
			Lock l(mMutex);
			for (auto& i : mBanks) {
				if (i.id == bankHandle) {
					VAE_INFO("Unloading bank %s", i.name.c_str())
					i = { }; // should free all the memory
					return Result::Success;
				}
			}
			VAE_INFO("Could not unload bank with handle %i", bankHandle)
			return Result::ElementNotFound;
		}
#pragma endregion bank_handling

	}; // Engine class
} } // namespace vae::core

#endif // VAE_ENGINE
