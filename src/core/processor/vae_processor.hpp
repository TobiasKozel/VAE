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

namespace vae { namespace core {
	/**
	 * @brief Non spatial voice processor
	 */
	struct Processor {
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
			SampleIndex frames, Size sampleRate
		) {
			VAE_PROFILER_SCOPE
			manager.forEachVoice([&](Voice& v, Size index) {
				if (v.bank != bank.id) { return true; }
				if (v.spatialized) { return true; }

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				if (signal.size() == 0) { return false; }

				VAE_ASSERT(signal.sampleRate == sampleRate || v.filtered);

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;

				target.setValidSize(frames); // mark mixer as active


				const auto gain = v.gain * source.gain;

				// TODO skip inaudible sounds
				v.audible = true;

				if (!v.filtered) {
					VAE_PROFILER_SCOPE
					// Basic rendering to all output channels w/o any effects
					v.started = true;
					const SampleIndex remaining = std::min(
						frames, SampleIndex(signal.size() - v.time)
					);
					for (int c = 0; c < target.channels(); c++) {
						const int channel = c % signal.channels();
						for (SampleIndex s = 0; s < remaining; s++) {
							target[c][s] += signal[channel][v.time + s] * gain;
						}
					}
					v.time += remaining; // progress time in voice
					return remaining == frames; // Finished if there are no samples left in source
				}

				// Filtered voice processing
				{
					VAE_PROFILER_SCOPE
					bool finished = false;

					auto& fd = manager.getVoiceFilter(index);

					if (!v.started) {
						// Initialize filter variables when first playing the voice
						for (int c = 0; c < Config::MaxChannels; c++) {
							fd.highpassScratch[c]	= 0;
							fd.lowpassScratch[c]	= signal[c % signal.channels()][0];
						}
						v.started = true;
					}

					// max samples we can read
					const Size countIn = signal.size();
					// fractional time, we need the value after the loop, so it's defined outside
					Sample position;

					// Playback speed taking samplerate into account
					const Sample speed = fd.speed * (Sample(signal.sampleRate) / Sample(sampleRate));

					for (int c = 0; c < target.channels(); c++) {
						const int channel = c % signal.channels();
						for (SampleIndex s = 0; s < frames; s++) {
							// Linear interpolation between two samples
							position = v.time + (s * speed) + fd.timeFract;
							const Sample lastPosition = std::floor(position);
							const Size lastIndex = (Size) lastPosition;
							const Size nextIndex = (Size) lastPosition + 1;

							if (countIn <= nextIndex) {
								finished = true;
								break;
							}

							Sample mix = position - lastPosition;
							// mix = 0.5 * (1.0 - cos((mix) * 3.1416)); // cosine interpolation, introduces new harmonics somehow
							const Sample last = signal[channel][lastIndex] * gain;
							const Sample next = signal[channel][nextIndex] * gain;
							// linear resampling, sounds alright enough
							const Sample in = last + mix * (next - last);


							//	* super simple lowpass and highpass filter
							// just lerps with a previous value
							const Sample lpd = in + fd.lowpass * (fd.lowpassScratch[c] - in);
							fd.lowpassScratch[c] = lpd;

							const Sample hps = fd.highpassScratch[c];
							const Sample hpd = hps + fd.highpass * (in - hps);
							fd.highpassScratch[c] = hpd;

							target[c][s] += (lpd - hpd);
						}
					}
					position += speed; // step to next sample
					v.time = std::floor(position); // split up in sample and fractional time
					fd.timeFract = position - v.time;
					return !finished;
				}
			});
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
