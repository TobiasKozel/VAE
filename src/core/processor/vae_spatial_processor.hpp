#ifndef _VAE_SPATIAL_PROCESSOR
#define _VAE_SPATIAL_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../vae_voice_manager.hpp"
#include "../vae_spatial_manager.hpp"
#include "../algo/vae_spcap.hpp"

#include "../../../external/glm/glm/gtc/matrix_transform.hpp"
#include "../../../external/tklb/src/types/audio/fft/TOouraFFT.hpp"
#include "../fs/vae_hrtf_loader.hpp"

namespace vae { namespace core {
	class SpatialProcessor {
		HRTF mHRTF;
		AudioBuffer mFrequencyDomain;
		AudioBuffer mTimeDomain;
		tklb::FFTOoura mFFT;
	public:
		SpatialProcessor() {
			mFrequencyDomain.resize(Config::MaxBlock / 2, 2);
			mTimeDomain.resize(Config::MaxBlock);
			mFFT.resize(128);
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
			manager.forEachVoice([&](Voice& v, Size vi) {
				if (v.bank != bank.id) { return true; }						// wrong bank
				if (!v.flags[Voice::Flags::spatialized]) { return true; }	// not spatialized

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				if (signal.size() == 0) { return false; }

				VAE_ASSERT(signal.sampleRate == sampleRate)					// needs resampling

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				v.flags[Voice::Flags::started] = true;

				if (!spatial.hasEmitter(v.emitter)) { // emitter missing
					v.time += remaining; // progress time in voice
					return remaining == frames;
				}


				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;
				auto& emitter = spatial.getEmitter(v.emitter);

				const Sample* in = signal[0] + v.time;

				target.setValidSize(frames); // mark mixer as active

				const auto gain = v.gain * source.gain;
				const Sample step = Sample(1) / Sample(frames);

				auto& lastPip = manager.getVoicePIP(vi);
				VoicePIP currentPip;
				bool audible = false;

				// Each listener gets the sound mixed in from it's position
				// ! this means using different configurations doesn't work !
				spatial.forEachListener([&](Listener& l, ListenerHandle li) {

					// samething as graphices, make the world rotate round the listener
					glm::mat4x3 lookAt = glm::lookAt(l.position, l.position + l.front, l.up);
					// listener is the world origin now
					Vec3 relativeDirection = emitter.position * lookAt;

					const Sample distance = glm::length(relativeDirection);
					relativeDirection /= distance;
					Sample distanceAttenuated = distance;

					if (distanceAttenuated < 0.05) {
						// Source is really close
						distanceAttenuated = gain;
						// no attenuation and technically no panning

					} else {
						distanceAttenuated = std::max(distanceAttenuated, Sample(1));
						distanceAttenuated = std::min(distanceAttenuated, Sample(1000000));
						distanceAttenuated = Sample(std::pow(distanceAttenuated / Sample(1.0), -Sample(1)));
						distanceAttenuated *= gain;
						if (distanceAttenuated < 0.001) { return; } // ! inaudible
					}

					audible = true;

					switch (l.configuration) {
					case Listener::Configuration::HRTF:
						if (v.flags[Voice::Flags::HRTF]) {
							// Only do hrtf when the voice also has it enabled
							Sample closest = std::numeric_limits<Sample>::max();
							constexpr Size Invalid = ~0;
							Size closestIndex = Invalid;
							for (Size i = 0; i < mHRTF.positions.size(); i++) {
								const auto& pos = mHRTF.positions[i];
								// TODO find distance2
								const Sample dist = glm::distance(pos.pos, relativeDirection);
								if (dist < closest) {
									closestIndex = i;
									closest = dist;
								}
							}
							if (closestIndex == Invalid) {
								return;
							}
							const auto& ir = mHRTF.positions[closestIndex].ir;

							for (int c = 0; c < 1; c++) {
								mTimeDomain.set(0);
								mTimeDomain.set(signal, remaining, v.time);
								mTimeDomain.setValidSize(mTimeDomain.size());
								mFFT.forward(mTimeDomain, mFrequencyDomain);

								auto re = mFrequencyDomain[0];
								auto im = mFrequencyDomain[1];
								const auto reA = ir[c][0];
								const auto imA = ir[c][1];
								for (Size i = 0; i < mFrequencyDomain.size(); i++) {
									const auto _reB = re[i];
									const auto _imB = im[i];
									re[i] += reA[i] * _reB - imA[i] * _imB;
									im[i] += reA[i] * _imB + imA[i] * _reB;
								}

								mFFT.back(mFrequencyDomain, mTimeDomain);
								for (Size i = 0; i < remaining; i++) {
									target[c][i] += mTimeDomain[0][i];
								}
							}


							return;
						}
					case Listener::Configuration::Headphones:
						{
							auto& currentVolumes = currentPip.listeners[li].volumes;
							auto& lastVolumes = lastPip.listeners[li].volumes;

							SPCAP::HeadphoneSPCAP.pan(
								relativeDirection, currentVolumes,
								distanceAttenuated, emitter.spread
							);

							if (v.time == 0) {
								// first time don't interpolate
								lastVolumes[0] = currentVolumes[0];
								lastVolumes[1] = currentVolumes[1];
							}

							// Copy audio and apply panning
							Sample t = 0;
							const Sample& lv1 = lastVolumes[0];
							const Sample& rv1 = lastVolumes[1];
							const Sample& lv2 = currentVolumes[0];
							const Sample& rv2 = currentVolumes[1];
							for (SampleIndex s = 0; s < remaining; s++) {
								const Sample sample = in[s];
								target[0][s] += sample * (lv1 + t * (lv2 - lv1));
								target[1][s] += sample * (rv1 + t * (rv2 - rv1));
								t += step;
							}
							return;
						}
					}
				});

				v.flags[Voice::Flags::audible] = audible;
				lastPip = std::move(currentPip);

				v.time += remaining; // progress time in voice
				return remaining == frames;
			});
		}

		Result loadHRTF(const char* path, const char* rootPath, Size sampleRate) {
			return HRTFLoader::load(
				path, rootPath, sampleRate, mHRTF
			);
		}
	};

	constexpr int _VAE_SPATIAL_PROCESSOR_SIZE = sizeof(SpatialProcessor);

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
