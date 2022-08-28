#ifndef _VAE_PROCESSOR
#define _VAE_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../voices/vae_voice.hpp"
#include "../voices/vae_voice_filter.hpp"
#include "../voices/vae_voice_pan.hpp"
#include "../vae_voice_manager.hpp"
#include <cmath>
#include <limits>

namespace vae { namespace core {
	/**
	 * @brief Non spatial voice processor
	 */
	class Processor {
		/**
		 * @brief Temporary filtered/looped signal TODO this will not work with parallel bank processing
		 */
		ScratchBuffer mScratchBuffer;
	public:
		Result init() {
			mScratchBuffer.resize(StaticConfig::MaxBlock, StaticConfig::MaxChannels);
			return Result::Success;
		}

		/**
		 * @brief Process a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 * @param sampleRate
		 * @return Voices renderd
		 */
		Size mix(
			VoiceManger& manager, Bank& bank,
			SampleIndex frames, Size sampleRate
		) {
			Size actuallyRendered = 0;
			TKLB_PROFILER_SCOPE_NAMED("Default Processor")
			manager.forEachVoice([&](Voice& v, Size index) {
				if (v.bank != bank.id) { return true; }
				if (v.spatialized) { return true; }
				TKLB_PROFILER_SCOPE_NAMED("Default Voice")

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				const SampleIndex signalLength = signal.size();

				if (signalLength == 0) { return false; }
				if (signal.sampleRate != sampleRate) {
					// TKLB_DEBUG("Spatial Voice samplerate mismatch. Enabled filter.")
					v.filtered = true; // implicitly filter to resample
				}

				v.time = v.time % signalLength;		// Keep signal in bounds before starting

				const auto signalChannels = signal.channels();
				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;
				const auto targetChannels = target.channels();
				const auto gain = v.gain * source.gain;

				// TODO skip inaudible sounds
				actuallyRendered++;
				v.audible = true;
				auto& pan = manager.getVoicePan(index);
				target.setValidSize(frames); // mark mixer as active

				if (!v.filtered) {
					TKLB_PROFILER_SCOPE_NAMED("Render Voice Basic")
					// Basic rendering to all output channels w/o any effects
					v.started = true;
					const SampleIndex needed = v.loop ? frames : tklb::min(frames, SampleIndex(signalLength - v.time));

					if (v.loop) {
						for (int c = 0; c < targetChannels; c++) {
							const int channel = c % signalChannels;
							for (SampleIndex s = 0; s < needed; s++) {
								target[c][s] +=
									signal[channel][((v.time + s) % signalLength)] * gain * pan.volumes[c];
							}
						}
					} else {
						// Having these seperate results in like a 3x speedup
						// TODO check this in a more diffictult to branchpredict scenario
						for (int c = 0; c < targetChannels; c++) {
							for (SampleIndex s = 0; s < needed; s++) {
							const int channel = c % signalChannels;
								target[c][s] +=
									signal[channel][v.time + s] * gain * pan.volumes[c];
							}
						}
					}

					v.time = v.time + frames;	// progress voice
					return needed == frames;	// Finished if there are no samples left in source
				}

				// Filtered voice processing
				{
					TKLB_PROFILER_SCOPE_NAMED("Render filtered Voice")
					auto& fd = manager.getVoiceFilter(index);

					if (!v.started) {
						// Initialize filter variables when first playing the voice
						for (int c = 0; c < StaticConfig::MaxChannels; c++) {
							fd.highpassScratch[c]	= 0;
							fd.lowpassScratch[c]	= signal[c % signalChannels][v.time];
						}
						v.started = true;
					}

					// fractional time, we need the value after the loop, so it's defined outside
					Real position;

					// Playback speed taking samplerate into account
					const Real speed = fd.speed * (Sample(signal.sampleRate) / Sample(sampleRate));
					const SampleIndex needed = v.loop ? frames : tklb::min(
						frames, SampleIndex((signalLength - v.time) / speed - fd.timeFract)
					);

					for (int c = 0; c < target.channels(); c++) {
						for (SampleIndex s = 0; s < needed; s++) {
						const int channel = c % signal.channels();
							// Linear interpolation between two samples
							position = v.time + (s * speed) + fd.timeFract;
							const Real lastPosition = SampleIndex(position);
							const Size lastIndex = (Size) lastPosition;
							const Size nextIndex = (Size) lastPosition + 1;

							Real mix = position - lastPosition;
							// mix = 0.5 * (1.0 - cos((mix) * 3.1416)); // cosine interpolation, introduces new harmonics somehow
							const Sample last = signal[channel][lastIndex % signalLength] * gain;
							const Sample next = signal[channel][nextIndex % signalLength] * gain;
							// linear resampling, sounds alright enough
							const Sample in = last + mix * (next - last);

							//	* super simple lowpass and highpass filter
							// just lerps with a previous value
							const Sample lf = fd.lowpass;
							const Sample lpd = in + lf * (fd.lowpassScratch[c] - in);
							fd.lowpassScratch[c] = lpd;

							const Sample hf = fd.highpass;
							const Sample hps = fd.highpassScratch[c];
							const Sample hpd = hps + hf * (in - hps);
							fd.highpassScratch[c] = hpd;

							target[c][s] += (lpd - hpd) * pan.volumes[c];
						}
					}
					position += speed; 					// step to next sample
					v.time = SampleIndex(position);		// split the signal in normal sample position
					fd.timeFract = position - v.time;	// and fractional time for the next block
					return needed == frames;			// we might have reached the end;					// is only true when exceeding signalLength and not looping
				}
			});
			return actuallyRendered;
		}
	};

	constexpr int __ProcessorSize = sizeof(Processor);

} } // vae::core

#endif // _VAE_PROCESSOR
