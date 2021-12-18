#ifndef _VAE_SPATIAL_PROCESSOR
#define _VAE_SPATIAL_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../vae_voice_manager.hpp"
#include "../vae_spatial_manager.hpp"
#include "../algo/vae_spcap.hpp"
#include "../algo/vae_search_hrtf.hpp"

#include "../fs/vae_hrtf_loader.hpp"
#include "../../../external/glm/glm/gtc/matrix_transform.hpp"
#include "../../../external/tklb/src/types/audio/fft/TOouraFFT.hpp"

namespace vae { namespace core {
	class SpatialProcessor {
		HRTF mHRTF;
		AudioBuffer mTimeDomain;
		Size mBufferPin = 0;
	public:
		SpatialProcessor() {
			mTimeDomain.resize(Config::MaxBlock);
		}
		/**
		 * @brief Process a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 * @param sampleRate
		 */
		void mix(
			VoiceManger& manager, Bank& bank,
			SpatialManager& spatial,
			SampleIndex frames, Size sampleRate
		) {
			VAE_PROFILER_SCOPE
			manager.forEachVoice([&](Voice& v, Size vi) {
				if (v.bank != bank.id) { return true; }						// wrong bank
				if (!v.spatialized) { return true; }	// not spatialized

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				if (signal.size() == 0) { return false; }

				VAE_ASSERT(signal.sampleRate == sampleRate)					// needs resampling

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				v.started = true;

				if (!spatial.hasEmitter(v.emitter)) {
					return false; // emitter missing, stop voice
				}

				auto& emitter = spatial.getEmitter(v.emitter);
				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;

				const Sample* const in = signal[0] + v.time;

				target.setValidSize(frames); // mark mixer as active

				const auto gain = v.gain * source.gain;

				auto& lastPip = manager.getVoicePIP(vi);
				VoicePIP currentPip;
				bool audible = false;

				// Each listener gets the sound mixed in from it's position
				// ! this means using different configurations doesn't work !
				spatial.forEachListener([&](Listener& l, ListenerHandle li) {
					VAE_PROFILER_SCOPE

					// * Attenuation calculation

					// samething as graphics, make the world rotate round the listener
					// TODO this should be possible without a 4x4 matrix?
					glm::mat4x4 lookAt = glm::lookAt(l.position, l.position + l.front, l.up);
					// listener is the world origin now
					Vec3 relativeDirection = (lookAt * glm::vec4(emitter.position, 1.f));


					const Sample distance = glm::length(relativeDirection);
					relativeDirection /= distance;
					Sample distanceAttenuated = distance;

					distanceAttenuated = std::max(distanceAttenuated, Sample(1));
					distanceAttenuated = std::min(distanceAttenuated, Sample(1000000));
					distanceAttenuated = Sample(std::pow(distanceAttenuated / Sample(1.0), -Sample(1)));
					distanceAttenuated *= gain;

					if (distanceAttenuated < 0.001) { return; } // ! inaudible

					audible = true;

					if (l.configuration == Listener::Configuration::HRTF &&v.HRTF && mHRTF.rate) {

						// * HRTF Panning

						VAE_PROFILER_SCOPE

						Size closestIndex = SearchHRTF::closest(mHRTF, relativeDirection);

						if (closestIndex == ~Size(0)) { return; } // ! no hrtf found?

						const auto& hrtf    = mHRTF.positions[closestIndex];
						const auto& irLeft  = hrtf.ir[0][0];
						const auto& irRight = hrtf.ir[1][0];
						const auto irLen    = hrtf.ir[0].size();

						for (int i = 0; i < remaining; i++) {
							Sample leftSum = 0;
							Sample rightSum = 0;
							mTimeDomain[0][mBufferPin] = in[i];

							for (int n = 0; n < irLen; n++) {
								const auto conv = mTimeDomain[0][(irLen + mBufferPin - n) % irLen];
								leftSum  += irLeft[n]  * conv;
								rightSum += irRight[n] * conv;
							}
							target[0][i] += leftSum  * distanceAttenuated;
							target[1][i] += rightSum * distanceAttenuated;

							mBufferPin = (mBufferPin + 1) % irLen;
						}
						return;

					}

					// * Normal SPCAP panning

					auto& currentVolumes = currentPip.listeners[li].volumes;
					auto& lastVolumes = lastPip.listeners[li].volumes;

					/**
					 * @brief Pan and mix templated lambda so we don't have to write this for each vonfig
					 * @param panner Get's a panner instance with pan() and speakers() function
					 */
					const auto pan = [&](const auto& panner) {
						panner.pan(
							relativeDirection, currentVolumes,
							distanceAttenuated, emitter.spread
						);

						if (v.time == 0) {
							// first time don't interpolate
							for (Size c = 0; c < panner.speakers(); c++) {
								lastVolumes[c] = currentVolumes[c];
							}
						}

						Sample t = 0;
						for (SampleIndex s = 0; s < remaining; s++) {
							const Sample sample = in[s];
							// lerp between last and current channel volumes
							// Not correct in terms of power convservation, but easy and efficient
							for (Size c = 0; c < panner.speakers(); c++) {
								target[c][s] += sample * (lastVolumes[c] + t * (currentVolumes[c] - lastVolumes[c]));
							}
							t += Sample(1) / Sample(frames);
						}
					};

					switch (l.configuration) {
						case Listener::Configuration::Headphones:	pan(SPCAP::HeadphoneSPCAP);	break;
						case Listener::Configuration::Stereo:		pan(SPCAP::StereroSPCAP);	break;
						case Listener::Configuration::Suround:		pan(SPCAP::SuroundSPCAP);	break;
						case Listener::Configuration::Quadrophonic:	pan(SPCAP::QuadSPCAP);		break;
						case Listener::Configuration::Mono:			pan(SPCAP::MonoSPCAP);		break;
					}
				});

				v.audible = audible;
				lastPip = std::move(currentPip);

				v.time += remaining; // progress time in voice
				return remaining == frames;
			});
		}

		Result loadHRTF(const char* path, const char* rootPath, Size sampleRate) {
			return HRTFLoader::load(path, rootPath, sampleRate, mHRTF);
		}
	};

	constexpr int _VAE_SPATIAL_PROCESSOR_SIZE = sizeof(SpatialProcessor);

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
