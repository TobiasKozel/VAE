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
#include "./voice/vae_voice.hpp"
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

		std::vector<Bank> mBanks;

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
		Engine(EngineConfig& config) : mConfig(config) { }

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
					auto& Source = bank.sources[i];
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
					// TODO
					// Stop sources
				}
				for (auto& i : event.on_start) {
					// TODO
					// kill every voice started from these events
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
		 * Load bank from filesystem
		 */
		Result loadBank(const char* path) {
			Bank bank;
			auto result = BankLoader::load(path, bank);
			if (result != Result::Success) { return result; }
			return loadBank(bank);
		}

		/**
		 * Load the bank from memory
		 * will move the bank object!
		 */
		Result loadBank(Bank& bank) {
			Lock l(mMutex);
			if (mBanks.size() < bank.id + 1) {
				mBanks.resize(bank.id + 1);
			}
			mBanks[bank.id] = std::move(bank);
			return Result::Success;
		}

		Result unloadBank(const char* path) {
			VAE_ASSERT(false)
			// TODO

			return Result::GenericFailure;
		}

		Result unloadBank(Bank& bank) {
			VAE_ASSERT(false)
			// TODO
			return Result::Success;
		}
#pragma endregion bank_handling

	}; // Engine class
} } // namespace vae::core

#endif // VAE_ENGINE
