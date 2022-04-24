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

namespace vae { namespace core {
	class SpatialProcessor {
		HRTF mHRTF;								///< Currently loaded HRTF, there can only be one
		HRTFLoader mHRTFLoader;					///< Struct to decode the hrtf
		HeapBuffer<VoiceHRTF> mVoiceHRTFs;		///< Working data for convolution
		/**
		 * @brief Temporary filtered/looped signal TODO this will not work with parallel bank processing
		 */
		ScratchBuffer mScratchBuffer;
	public:
		Result init(Size hrtfVoices) {
			VAE_PROFILER_SCOPE_NAMED("Spatial Processor Init")
			mVoiceHRTFs.resize(hrtfVoices);
			mScratchBuffer.resize(StaticConfig::MaxBlock);
			return Result::Success;
		}

		/**
		 * @brief Process a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 * @param sampleRate
		 * @return Number of voices mixed
		 */
		Size mix(
			VoiceManger& manager, Bank& bank,
			SpatialManager& spatial,
			SampleIndex frames, Size sampleRate
		) {
			Size actuallyRendered = 0;
			VAE_PROFILER_SCOPE_NAMED("Spatial Processor")
			manager.forEachVoice([&](Voice& v, Size vi) {
				if (v.bank != bank.id) { return true; }		// wrong bank
				if (!v.spatialized) { return true; }		// not spatialized
				VAE_PROFILER_SCOPE_NAMED("Spatial Voice")
				if (!spatial.hasEmitter(v.emitter)) {
					VAE_DEBUG("Spatial voice is missing emitter")
					return false; // ! needs emitter
				}

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				const auto signalLength = signal.size();

				if (signalLength == 0) { return false; }	// ! no signal

				v.time = v.time % signalLength;		// Keep signal in bounds before starting

				if (signal.sampleRate != sampleRate) {
					// VAE_DEBUG("Spatial Voice samplerate mismatch. Enabled filter.")
					v.filtered = true; // implicitly filter to resample
				}

				auto& emitter = spatial.getEmitter(v.emitter);
				auto& target = bank.mixers[v.mixer].buffer;
				const Sample gain = v.gain * source.gain;
				auto& l = spatial.getListeners()[v.listener];

				Real distanceAttenuated;
				Vec3 relativeDirection;
				// * Attenuation calculation
				{
					VAE_PROFILER_SCOPE_NAMED("Attenuation calculation")
					// samething as graphics, make the world rotate round the listener
					// TODO this should be possible without a 4x4 matrix?
					glm::mat4x4 lookAt = glm::lookAt(l.position, l.position + l.front, l.up);
					// listener is the world origin now
					relativeDirection = (lookAt * glm::vec4(emitter.position, 1.f));


					const Real distance = std::max(glm::length(relativeDirection), 0.1f);
					relativeDirection /= distance;

					if (v.attenuate) {
						distanceAttenuated = distance;
						distanceAttenuated = std::max(distanceAttenuated, Real(1)); // we don't want to get louder than 1
						distanceAttenuated = Real(1) / distanceAttenuated;
					} else {
						distanceAttenuated = 1.0;
					}
					distanceAttenuated *= gain;
				}

				if (distanceAttenuated < StaticConfig::MinVolume) {
					return true; // ! inaudible
					// TODO maybe progress still progress time?
				}
				actuallyRendered++;
				target.setValidSize(frames); // mark mtarget ixer as active
				v.audible = true;

				// * Filtering and looping logic

				// TODO This thing is littered with branches, maybe needs some cleanup

				const Sample* in;				// The filtered, looped original signal used for panning later. We only do mono signals
				SampleIndex remaining = frames;	// playback speed and looping affects this
				bool finished = false; 			// the return value of this function stops the voice

				if (v.filtered) {
					VAE_PROFILER_SCOPE_NAMED("Voice Filter")
					auto& fd = manager.getVoiceFilter(vi);

					if (!v.started) {
						// Initialize filter variables when first playing the voice
						fd.highpassScratch[0]	= 0;
						fd.lowpassScratch[0]	= signal[0][v.time];
					}

					// Playback speed taking samplerate into account
					const Sample speed = fd.speed * (Sample(signal.sampleRate) / Sample(sampleRate));

					if (!v.loop) {
						// If we're not looping, end time calculation is a bit more complex
						remaining = std::min(
							frames,
							SampleIndex(std::floor((signalLength - v.time) / speed - fd.timeFract))
						);
						finished = remaining != frames;	// we might have reached the end
					}

					// fractional time, we need the value after the loop, so it's defined outside
					Real position;
					for (SampleIndex s = 0; s < frames; s++) {
						// Linear interpolation between two samples
						position = v.time + (s * speed) + fd.timeFract;
						const Real lastPosition = std::floor(position);

						Size lastIndex = (Size) lastPosition;
						Size nextIndex = (Size) lastIndex + 1;
						// TODO this kills vectorization
						if (signalLength <= lastIndex) { lastIndex = 0; }
						if (signalLength <= nextIndex) { nextIndex = 0; }

						Real mix = position - lastPosition;
						// mix = 0.5 * (1.0 - cos((mix) * 3.1416)); // cosine interpolation, introduces new harmonics, not really better than linear

						const Sample last = signal[0][lastIndex];
						const Sample next = signal[0][nextIndex];
						// linear resampling, sounds alright enough
						const Sample in = (last + mix * (next - last)) * gain;

						//	* super simple lowpass and highpass filter
						// just lerps with a previous value
						const Sample lpd = in + fd.lowpass * (fd.lowpassScratch[0] - in);
						fd.lowpassScratch[0] = lpd;

						const Sample hps = fd.highpassScratch[0];
						const Sample hpd = hps + fd.highpass * (in - hps);
						fd.highpassScratch[0] = hpd;

						mScratchBuffer[0][s] = (lpd - hpd);
					}
					position += speed; 					// step to next sample
					v.time = (SampleIndex) std::floor(position);		// split the signal in normal sample position
					fd.timeFract = position - v.time;	// and fractional time for the next block
					v.time = v.time;					// set index back
					in = mScratchBuffer[0];				// set the buffer to use for panning
				} else {
					VAE_PROFILER_SCOPE_NAMED("Non filtered Voice")
					if (v.loop) {
						VAE_PROFILER_SCOPE_NAMED("Loop")
						// put the looped signal in scratch buffer eventhough we're not filtering
						// so panning doesn't need to worry about looping
						for (SampleIndex s = 0, i = v.time; s < frames; s++, i++) {
							// reached the end so reset index, this is slighlty faster than modulo
							if (signalLength <= i) { i = 0; }
							mScratchBuffer[0][s] = signal[0][i];
						}
						v.time = (v.time + frames);	// progress the time
						in = mScratchBuffer[0];		// set buffer for panning
						finished = false;			// never stop the voice
					} else {
						VAE_PROFILER_SCOPE_NAMED("No Loop")
						// Not filtering or looping
						// Means we can use the original signal buffer but need to
						// set the remaining samples so we don't run over the signal end
						remaining = std::min(
							frames, SampleIndex(signalLength - v.time
						));
						in = signal[0] + v.time;
						finished = remaining != frames;	// we might have reached the end
						v.time += remaining;			// progress time in voice
					}
				}

				if (l.configuration == SpeakerConfiguration::HRTF && v.HRTF && mHRTF.rate) {
					// * HRTF Panning
					VAE_ASSERT(vi < mVoiceHRTFs.size()) // only the lower voice can use hrtfs
					VAE_PROFILER_SCOPE_NAMED("Render HRTF")

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

				} else {
					VAE_PROFILER_SCOPE_NAMED("Render SPCAP")
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
						// This is actually constexpr but not according to clangd
						constexpr Size channels = Size(StaticConfig::MaxChannels);
						panner.pan(
							relativeDirection, currentVolumes,
							distanceAttenuated, emitter.spread
						);

						if (!v.started) {
							// first time don't interpolate
							for (Size c = 0; c < channels; c++) {
								lastVolumes[c] = currentVolumes[c];
							}
						}
						VAE_PROFILER_SCOPE_NAMED("Apply SPCAP")
						Sample t = 0;
						for (SampleIndex s = 0; s < remaining; s++) {
							const Sample sample = in[s];
							// lerp between last and current channel volumes
							// Not correct in terms of power convservation, but easy and efficient
							for (Size c = 0; c < channels; c++) {
								// target[c][s] += sample * (lastVolumes[c] + t * (currentVolumes[c] - lastVolumes[c]));
								target[c][s] += sample * currentVolumes[c];
							}
							t += Sample(1) / Sample(frames);
						}
					};

					switch (l.configuration) {
						case SpeakerConfiguration::HRTF:
						case SpeakerConfiguration::Headphones:		pan(SPCAP::HeadphoneSPCAP);	break;
						case SpeakerConfiguration::Stereo:			pan(SPCAP::StereroSPCAP);	break;
						case SpeakerConfiguration::Suround:			pan(SPCAP::SuroundSPCAP);	break;
						case SpeakerConfiguration::Quadrophonic:	pan(SPCAP::QuadSPCAP);		break;
						case SpeakerConfiguration::Mono:			pan(SPCAP::MonoSPCAP);		break;
					}

					lastPan = std::move(currentPan);
				}
				v.started = true;
				if (finished) {
					emitter.autoplaying = false;
					return false;
				}
				return true;
			});
			return actuallyRendered;
		}

		Result loadHRTF(const char* path, Size length, const char* rootPath, Size sampleRate) {
			Result result = mHRTFLoader.load(path, length, rootPath, sampleRate, mHRTF);
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
