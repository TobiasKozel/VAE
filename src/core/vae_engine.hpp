#ifndef _VAE_ENGINE
#define _VAE_ENGINE

#include <cstring>
#include <vector>

#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "../../external/tklb/src/util/TMath.hpp"


#include "../../include/vae/vae.hpp"
// #include "./device/vae_rtaudio.hpp"
#include "./device/vae_portaudio.hpp"
// #include "./vae_emitter.hpp"
#include "../wrapped/vae_profiler.hpp"
#include "./vae_types.hpp"
#include "./vae_bus_events.hpp"
#include "./voice/vae_voice.hpp"
#include "./pod/vae_bank.hpp"

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

		HeapBuffer<Voice> mVoices;
		HeapBuffer<Voice> mVirtualVoices;
		// tklb::HandleBuffer<Clip> mClips = { &Clip::id };

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
		Engine(EngineConfig& config) {
			mConfig = config;

			// Backend& backend = CurrentBackend::instance();
			// mDevice = backend.createDevice();
			// mDevice->setSync([&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
			// 	callback(fromDevice, toDevice);
			// });
			// mDevice->openDevice();
		}

		~Engine() {
			TKLB_DELETE(mDevice);
		}


		/**
		 * Main mechanism to start and stop sounds
		 */
		Result fireEvent(BankHandle bank, EventHandle event) {
			auto& e = mBanks[bank].events[event];
			switch (e.type)
			{
			case Event::EventType::start:
				for (auto& i : e.sources) {
					// TODO
					// start sources
					// associate event with sounds
					// fire on_end when all sounds are done
				}
				break;
			case Event::EventType::stop:
				for (auto& i : e.sources) {
					// TODO
					// Stop sources
				}
				// directly fire on end since the sounds should now
				// be stopped maybe?
				for (auto& i : e.on_end) {
					fireEvent(bank, i);
				}
				break;
			case Event::EventType::emit:
				if (mConfig.eventCallback == nullptr) { break; }
				EventCallbackData data;
				data.bank = bank;
				data.event = event;
				data.name = e.name.c_str();
				data.payload = mConfig.eventCallbackPayload;
				mConfig.eventCallback(data);
				break;
			default:
				break;
			}

			// Always trigger on start events
			for (auto& i : e.on_start) {
				fireEvent(bank, i);
			}
			return Result::Success;
		}


		Result loadBank(const char* path) {
			Bank bank;
			auto result = Bank::Loader{}(path, bank);
			if (result) { return result; }
			Lock l(mMutex);
			if (mBanks.size() < bank.id + 1) {
				mBanks.resize(bank.id + 1);
			}
			mBanks[bank.id] = std::move(bank);
			return Result::Success;
		}

		Result unloadBank(const char* path) {
			// for (Size i = 0; i < mBanks.size(); i++) {
			// 	if (strcmp(mBanks[i].path, path) == 0) {
			// 		return unloadBank(mBanks[i]);
			// 	}
			// }
			return Result::GenericFailure;
		}

		Result unloadBank(Bank& bank) {
			// Lock l(mMutex);
			// // TODO
			// for (Size i = 0; i < bank.sources.size(); i++) {

			// }

			// for (Size i = 0; i < bank.events.size(); i++) {

			// }
			// mBanks.remove(&bank);
			return Result::Success;
		}
	};
} } // namespace vae::core

#endif // VAE_ENGINE
