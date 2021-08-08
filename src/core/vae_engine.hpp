#ifndef VAE_ENGINE
#define VAE_ENGINE


// #define TKLB_NO_ASSERT

#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "./device/vae_rtaudio.hpp"
#include "./vae_emitter.hpp"
namespace vae { namespace core {


	class Engine {
		using Mutex = tklb::SpinLock;
		using CurrentBackend = BackendRtAudio;

		static constexpr int EmitterSize = sizeof(Emitter);

		Device* mDevice;

		size_t mTime = 0; // Global time in samples

		tklb::HeapBuffer<Emitter> mEmitters;
		tklb::HeapBuffer<Clip> mClips; // clips in memory
		Listener mListener;
		Mutex mLock;

		void callback(const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
			toDevice.set(0.0f);
			const size_t samples = toDevice.validSize();
			mLock.lock();
			const int count = mEmitters.size();
			for (int index = 0; index < count; index++) {
				auto& i = mEmitters[index];

				if (!(i.state[Emitter::playing])) { continue; }

				const size_t startTime = i.time;
				const auto& buffer = i.clip->data;
				const size_t totalLength = buffer.size();
				const size_t length = std::min(samples, totalLength - startTime);

				if (!i.state[Emitter::virt]) {
					for (int c = 0; c < toDevice.channels(); c++) {
						int channel = c % buffer.channels();
						for (size_t s = 0; s < length; s++) {
							toDevice[c][s] += buffer[c][startTime + s];
						}
					}
				}

				if (totalLength <= startTime + length) {
					i.time = 0;;
					if (!i.state[Emitter::loop]) {
						i.state[Emitter::playing] = false; // stop if not looping
					}
				} else {
					i.time = startTime + length;
				}
			}
			mLock.unlock();

			mTime += samples;
		}

	public:
		Engine() {
			Backend& backend = CurrentBackend::instance();

			Device::SyncCallback wrappedCallback =
			[&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
				callback(fromDevice, toDevice);
			};

			mDevice = backend.createDevice();
			mDevice->setSync(wrappedCallback);
			mDevice->openDevice();
		}

		~Engine() {
			delete mDevice;
		}

		Listener& listener() { return mListener; }


	};
} } // namespace vae::core

#endif // VAE_ENGINE
