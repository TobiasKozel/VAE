#ifndef _VAE_ENGINE
#define _VAE_ENGINE

#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/TSpinLock.hpp"
#include "../../external/tklb/src/util/TMath.hpp"
#include "./device/vae_rtaudio.hpp"
#include "./vae_emitter.hpp"
#include "../wrapped/vae_profiler.hpp"
#include "./vae_types.hpp"

namespace vae { namespace core {

	class Engine {
		using Mutex = tklb::SpinLock;
		using CurrentBackend = BackendRtAudio;

		static constexpr int EmitterSize = sizeof(Emitter);

		tklb::HandleBuffer<Emitter> mEmitters = { &Emitter::clip };
		tklb::HandleBuffer<Clip> mClips = { &Clip::id };

		Device* mDevice;

		SampleIndex mTime = 0; // Global time in samples

		Listener mListener;
		Mutex mLock;

		void callback(const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
			toDevice.set(0.0f);
			const auto samplesNeded = toDevice.validSize();
			mLock.lock();
			const auto count = mEmitters.size();

			const auto speakerL = glm::normalize(glm::vec2(-1, 0.5));
			const auto speakerR = glm::normalize(glm::vec2(+1, 0.5));
			const glm::mat2x2 speakers(
				speakerL.x, speakerL.y,
				speakerR.x, speakerR.y
			);
			const auto matrix = glm::inverse(speakers);

			for (types::Size index = 0; index < count; index++) {

				if (mEmitters.getLastFree() == index) { continue; }
				auto& i = mEmitters[index];

				if (!(i.state[Emitter::ready]))   { continue; }
				if (!(i.state[Emitter::playing])) { continue; }
				Clip* clip = mClips.at(i.clip);
				if (clip == nullptr) { continue; }
				auto& buffer = clip->data;
				const auto totalLength = buffer.size();

				const auto startTime = i.time;

				if (!i.state[Emitter::virt]) {
					const auto direction = glm::normalize(glm::vec2(i.position.x, i.position.y));
					auto pan = direction * matrix;
					pan[1] = tklb::clamp<float>(+direction.x, 0, 1.0);
					pan[0] = tklb::clamp<float>(-direction.x, 0, 1.0);
					// pan[0] = 1;
					// pan[1] = 1;
					// pan[0] = std::min(std::max(-i.position.x, 0.f));

					if (i.state[Emitter::loop]) {
						for (int c = 0; c < toDevice.channels(); c++) {
							const int channel = c % buffer.channels();
							for (size_t s = 0; s < samplesNeded; s++) {
								toDevice[c][s] += buffer[channel][(startTime + s) % totalLength] * pan[c];
							}
						}
					} else {
						const auto length = std::min(samplesNeded, totalLength - startTime);
						for (int c = 0; c < toDevice.channels(); c++) {
							const int channel = c % buffer.channels();
							for (size_t s = 0; s < length; s++) {
								toDevice[c][s] += buffer[channel][startTime + s] * pan[c];
							}
						}
					}
				}

				if (!i.state[Emitter::loop] && totalLength <= startTime + samplesNeded) {
					// stop if not looping and end is reached
					i.time = 0;
					i.state[Emitter::playing] = false;
				} else {
					// move time forward
					i.time = startTime + samplesNeded % totalLength;
				}
			}
			mLock.unlock();

			mTime += samplesNeded;
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
