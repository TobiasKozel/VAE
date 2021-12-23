#ifndef _VAE_SPATIAL_PROCESSOR
#define _VAE_SPATIAL_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../voices/vae_voice.hpp"
#include "../voices/vae_voice_filter.hpp"
#include "../voices/vae_voice_pan.hpp"
#include "../voices/vae_voice_hrtf.hpp"
#include "../vae_voice_manager.hpp"
#include "../vae_spatial_manager.hpp"
#include "../algo/vae_spcap.hpp"
#include "../algo/vae_hrtf_util.hpp"

#include "../fs/vae_hrtf_loader.hpp"
#include "../../../external/glm/glm/gtc/matrix_transform.hpp"
#include "../../../external/tklb/src/types/audio/fft/TOouraFFT.hpp"

namespace vae { namespace core {
	class SpatialProcessor {
		HRTF mHRTF;								///< Currently loaded HRTF, there can only be one
		HRTFLoader mHRTFLoader;					///< Struct to decode the hrtf
		HeapBuffer<VoiceHRTF> mVoiceHRTFs;		///< Working data for convolution
		AudioBuffer mFilteredSignal;			///< Temporary filtered signal TODO this will not work with parallel bank processing
	public:
		Result init(Size hrtfVoices) {
			VAE_PROFILER_SCOPE
			mVoiceHRTFs.resize(hrtfVoices);
			mFilteredSignal.resize(Config::MaxBlock);
			return Result::Success;
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
				if (v.bank != bank.id) { return true; }		// wrong bank
				if (!v.spatialized) { return true; }		// not spatialized

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				if (signal.size() == 0) { return false; }	// ! no signal
				VAE_ASSERT(signal.sampleRate == sampleRate || v.filtered)	// ! needs resampling
				if (!spatial.hasEmitter(v.emitter)) { return false; } // ! needs emitter

				const auto& emitter = spatial.getEmitter(v.emitter);

				auto& target = bank.mixers[v.mixer].buffer;
				target.setValidSize(frames); // mark mixer as active

				const auto gain = v.gain * source.gain;

				auto& l = spatial.getListeners()[v.listener];

				// * Attenuation calculation

				// samething as graphics, make the world rotate round the listener
				// TODO this should be possible without a 4x4 matrix?
				glm::mat4x4 lookAt = glm::lookAt(l.position, l.position + l.front, l.up);
				// listener is the world origin now
				Vec3 relativeDirection = (lookAt * glm::vec4(emitter.position, 1.f));


				const Sample distance = std::max(glm::length(relativeDirection), 0.1f);
				relativeDirection /= distance;
				Sample distanceAttenuated = distance;

				distanceAttenuated = std::max(distanceAttenuated, Sample(1));
				distanceAttenuated = std::min(distanceAttenuated, Sample(1000000));
				distanceAttenuated = Sample(std::pow(distanceAttenuated / Sample(1.0), -Sample(1)));
				distanceAttenuated *= gain;

				if (distanceAttenuated < Config::MinVolume) { return true; } // ! inaudible
				v.audible = true;

				/**
				 * @brief Start of the relevant signal
				 * Spatialized voices only use about the first channel
				 */
				const Sample* in;
				SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				bool finished = false;
				if (v.filtered) {
					VAE_PROFILER_SCOPE
					remaining = frames; // playback speed affects this later on
					auto& fd = manager.getVoiceFilter(vi);

					if (!v.started) {
						// Initialize filter variables when first playing the voice
						fd.highpassScratch[0]	= 0;
						fd.lowpassScratch[0]	= signal[0][v.time];
					}

					const Size countIn = signal.size(); // max samples we can read
					// fractional time, we need the value after the loop, so it's defined outside
					Sample position;
					// Playback speed taking samplerate into account
					const Sample speed = fd.speed * (Sample(signal.sampleRate) / Sample(sampleRate));
					mFilteredSignal.set(); // clear shared buffer
					for (SampleIndex s = 0; s < frames; s++) {
						// Linear interpolation between two samples
						position = v.time + (s * speed) + fd.timeFract;
						const Sample lastPosition = std::floor(position);
						const Size lastIndex = (Size) lastPosition;
						const Size nextIndex = (Size) lastPosition + 1;

						if (countIn <= nextIndex) {
							remaining = s;
							finished = true;
							break;
						}

						Sample mix = position - lastPosition;
						// mix = 0.5 * (1.0 - cos((mix) * 3.1416)); // cosine interpolation, introduces new harmonics somehow
						const Sample last = signal[0][lastIndex] * gain;
						const Sample next = signal[0][nextIndex] * gain;
						// linear resampling, sounds alright enough
						const Sample in = last + mix * (next - last);


						//	* super simple lowpass and highpass filter
						// just lerps with a previous value
						const Sample lpd = in + fd.lowpass * (fd.lowpassScratch[0] - in);
						fd.lowpassScratch[0] = lpd;

						const Sample hps = fd.highpassScratch[0];
						const Sample hpd = hps + fd.highpass * (in - hps);
						fd.highpassScratch[0] = hpd;

						mFilteredSignal[0][s] = (lpd - hpd);
					}
					position += speed; // step to next sample
					v.time = std::floor(position); // split up in sample and fractional time
					fd.timeFract = position - v.time;
					in = mFilteredSignal[0];
				} else {
					in = signal[0] + v.time;
					finished = remaining != frames;
					v.time += remaining; // progress time in voice
				}


				if (l.configuration == Listener::Configuration::HRTF &&v.HRTF && mHRTF.rate) {
					// * HRTF Panning
					VAE_ASSERT(vi < mVoiceHRTFs.size()) // only the lower voice can use hrtfs
					VAE_PROFILER_SCOPE

					Size closestIndex = HRTFUtil::closest(mHRTF, relativeDirection);

					if (closestIndex == ~Size(0)) { return true; } // ! no hrtf found?

					auto& hrtfVoice = mVoiceHRTFs[vi];

					if (!v.started) { // clear old data
						hrtfVoice.convolutionIndex = 0;
						hrtfVoice.convolutionBuffer.set();
					}

					HRTFUtil::apply(
						mHRTF.positions[closestIndex],
						hrtfVoice, remaining, target, in, distanceAttenuated
					);

				}

				{
					VAE_PROFILER_SCOPE
					// * Normal SPCAP panning
					auto& lastPan = manager.getVoicePan(vi);
					VoicePan currentPan;
					auto& currentVolumes = currentPan.volumes;
					auto& lastVolumes = lastPan.volumes;

					/**
					* @brief Pan and mix templated lambda so we don't have to write this for each vonfig
					* @param panner Get's a panner instance with pan() and speakers() function
					*/
					const auto pan = [&](const auto& panner) {
						panner.pan(
							relativeDirection, currentVolumes,
							distanceAttenuated, emitter.spread
						);

						if (!v.started) {
							// first time don't interpolate
							for (Size c = 0; c < panner.speakers; c++) {
								lastVolumes[c] = currentVolumes[c];
							}
						}

						Sample t = 0;
						for (SampleIndex s = 0; s < remaining; s++) {
							const Sample sample = in[s];
							// lerp between last and current channel volumes
							// Not correct in terms of power convservation, but easy and efficient
							for (Size c = 0; c < panner.speakers; c++) {
								target[c][s] += sample * (lastVolumes[c] + t * (currentVolumes[c] - lastVolumes[c]));
							}
							t += Sample(1) / Sample(frames);
						}
					};

					switch (l.configuration) {
						case Listener::Configuration::HRTF:
						case Listener::Configuration::Headphones:	pan(SPCAP::HeadphoneSPCAP);	break;
						case Listener::Configuration::Stereo:		pan(SPCAP::StereroSPCAP);	break;
						case Listener::Configuration::Suround:		pan(SPCAP::SuroundSPCAP);	break;
						case Listener::Configuration::Quadrophonic:	pan(SPCAP::QuadSPCAP);		break;
						case Listener::Configuration::Mono:			pan(SPCAP::MonoSPCAP);		break;
					}

					lastPan = std::move(currentPan);

				}
				v.started = true;
				return !finished;
			});
		}

		Result loadHRTF(const char* path, const char* rootPath, Size sampleRate) {
			Result result = mHRTFLoader.load(path, rootPath, sampleRate, mHRTF);
			if (result != Result::Success) { return result; }
			for (auto& i : mVoiceHRTFs) {
				i.convolutionBuffer.resize(mHRTF.irLength);
				i.convolutionBuffer.set();
			}
			return Result::Success;
		}
	};

	constexpr int _VAE_SPATIAL_PROCESSOR_SIZE = sizeof(SpatialProcessor);

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
