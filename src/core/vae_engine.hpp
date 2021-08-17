#ifndef VAE_ENGINE
#define VAE_ENGINE

#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "./device/vae_rtaudio.hpp"
#include "./vae_emitter.hpp"
#include "../wrapped/vae_profiler.hpp"

namespace vae { namespace core {

	class Engine {
		using Mutex = tklb::SpinLock;
		using CurrentBackend = BackendRtAudio;

		static constexpr int EmitterSize = sizeof(Emitter);

		tklb::HandleBuffer<Emitter> mEmitters = { &Emitter::clip };
		tklb::HandleBuffer<Clip> mClips = { &Clip::id };

		Device* mDevice;

		size_t mTime = 0; // Global time in samples

		Listener mListener;
		Mutex mLock;

		void callback(const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
			toDevice.set(0.0f);
			const size_t samples = toDevice.validSize();
			mLock.lock();
			const int count = mEmitters.size();
			for (int index = 0; index < count; index++) {
				if (mEmitters.getLastFree() == index) { continue; }
				auto& i = mEmitters[index];

				if (!(i.state[Emitter::playing])) { continue; }

				const size_t startTime = i.time;
				Clip* clip = mClips.at(i.clip);
				if (clip == nullptr) { continue; }
				auto& buffer = clip->data;
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
					i.time = 0;
					if (!i.state[Emitter::loop]) {
						i.state[Emitter::playing] = false; // stop if not looping
					}
				} else {
					i.time = startTime + length;
				}
			}
			mLock.unlock();

			mTime += samples;
			VAE_PROFILER_FRAME_MARK
		}

	public:
		Engine() {
			Backend& backend = CurrentBackend::instance();
			mDevice = backend.createDevice();
			mDevice->setSync([&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
				callback(fromDevice, toDevice);
			});
			mDevice->openDevice();
		}

		~Engine() {
			TKLB_DELETE(mDevice);
		}

		tklb::Handle createEmitter() {
			return mEmitters.create();
		}

		bool destroyEmitter(tklb::Handle h) {
			return mEmitters.pop(h);
		}

		Emitter* getEmitter(tklb::Handle h) {
			return mEmitters.at(h);
		}

		tklb::Handle createClip() {
			return mClips.create();
		}

		bool destroyClip(tklb::Handle h) {
			return mClips.pop(h);
		}

		Clip* getClip(tklb::Handle h) {
			return mClips.at(h);
		}

		Listener& listener() { return mListener; }
	};
} } // namespace vae::core

#endif // VAE_ENGINE
