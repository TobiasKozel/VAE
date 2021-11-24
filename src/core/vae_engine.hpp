#ifndef _VAE_ENGINE
#define _VAE_ENGINE

#include <cstring>
#include <vector>

#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "../../external/tklb/src/util/TMath.hpp"

#include "../wrapped/vae_profiler.hpp"

#include "./vae_types.hpp"

// #include "./device/vae_rtaudio.hpp"
#include "./device/vae_portaudio.hpp"

// #include "./vae_emitter.hpp"
#include "./vae_bus_events.hpp"
#include "./voice/vae_voice_manager.hpp"
#include "./fs/vae_bank_loader.hpp"
#include "./vae_util.hpp"

namespace vae { namespace core {

	class Engine {
		using Mutex = tklb::SpinLock;
		using Lock = tklb::LockGuard<Mutex>;
		using CurrentBackend = BackendPortAudio;

		/**
		 * Main messaging instance, most objects hold a
		 * reference to this instead of the engine itself
		 */
		EventBus mBus;
		EngineConfig mConfig;

		std::vector<Bank> mBanks;		// All the currently loaded banks
		VoiceManger mVoiceManager;
		Mixer mMaster;					// Master channel which will be output

		Device* mDevice;

		SampleIndex mTime = 0; // Global time in samples

		// Listener mListener;
		Mutex mMutex;

		void callback(const AudioBuffer& fromDevice, AudioBuffer& toDevice) {
			VAE_PROFILER_FRAME_MARK
		}

		Engine(const Engine&) = delete;
		Engine(const Engine*) = delete;
		Engine(Engine&&) = delete;
		Engine& operator= (const Engine&) = delete;
		Engine& operator= (Engine&&) = delete;

	public:
		Engine(
			EngineConfig& config
		) : mConfig(config), mVoiceManager(config) { }

		~Engine() {
			// TKLB_DELETE(mDevice);
		}

		Result init() {
			// Backend& backend = CurrentBackend::instance();
			// mDevice = backend.createDevice();
			// mDevice->setSync([&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
			// 	callback(fromDevice, toDevice);
			// });
			// mDevice->openDevice();
		}


		/**
		 * @brief Main mechanism to start and stop sounds
		 *
		 * @param bankHandle
		 * @param eventHandle
		 * @param emitterHandle
		 * @return Result
		 */
		Result fireEvent(
			BankHandle bankHandle, EventHandle eventHandle,
			EmitterHandle emitterHandle = InvalidHandle)
		{
			auto& bank = mBanks[bankHandle];
			auto& event = bank.events[eventHandle];
			switch (event.type) {
			case Event::EventType::start:
				for (auto& i : event.sources) {
					mVoiceManager.play(i, eventHandle, emitterHandle);
					// TODO
					// start sources
					// associate event with sounds
					// fire on_end when all sounds are done
				}

				for (auto& i : event.on_start) {
					fireEvent(bankHandle, i, emitterHandle);
				}
				break;
			case Event::EventType::stop:
				for (auto& i : event.sources) {
					// Kill every voice started from this source
					mVoiceManager.stopFromSource(i, emitterHandle);
				}
				for (auto& i : event.on_start) {
					// kill every voice started from these events
					mVoiceManager.stopFromEvent(i, emitterHandle);
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
				mConfig.eventCallback(data);
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
			Bank bank;
			auto result = BankLoader::load(path, bank);
			if (result != Result::Success) { return result; }
			return loadBank(bank);
		}

		/**
		 * @brief Load bank from memory
		 * Locks audio thread
		 * @param bank Moved and now owned by the engine
		 * @return Result
		 */
		Result loadBank(Bank& bank) {
			Lock l(mMutex);
			if (mBanks.size() < bank.id + 1) {
				mBanks.resize(bank.id + 1);
			}
			mBanks[bank.id] = std::move(bank);
			// todo update master channel
			// and init effects
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

		/**
		 * @brief Unload bank from path
		 * Locks audio thread
		 * @param path
		 * @return Result
		 */
		Result unloadBank(const char* path) {
			Lock l(mMutex);
			for (auto& i : mBanks) {
				if (strcmp(path, i.path.c_str()) == 0) {
					i = { }; // should free all the memory
					return Result::Success;
				}
			}
			return Result::ElementNotFound;
		}

		/**
		 * @brief Unload bank from handle
		 * Locks audio thread
		 * @param bankHandle
		 * @return Result
		 */
		Result unloadBank(BankHandle bankHandle) {
			Lock l(mMutex);
			for (auto& i : mBanks) {
				if (i.id == bankHandle) {
					i = { }; // should free all the memory
					return Result::Success;
				}
			}
			return Result::ElementNotFound;
		}
#pragma endregion bank_handling

	}; // Engine class
} } // namespace vae::core

#endif // VAE_ENGINE
