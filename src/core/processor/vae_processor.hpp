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
				if (signal.sampleRate == sampleRate) {
					v.filtered = true; // implicitly filter to resample
				}

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;
				const auto gain = v.gain * source.gain;

				// TODO skip inaudible sounds
				v.audible = true;
				auto& pan = manager.getVoicePan(index);
				const auto signalLength = signal.size();
				const auto signalChannels = signal.channels();
				const auto targetChannels = target.channels();
				target.setValidSize(frames); // mark mixer as active

				if (!v.filtered) {
					VAE_PROFILER_SCOPE
					// Basic rendering to all output channels w/o any effects
					v.started = true;
					SampleIndex remaining;

					if (v.loop) {
						remaining = frames;
					} else {
						remaining = std::min( frames, SampleIndex(signal.size() - v.time));
					}

					for (int c = 0; c < targetChannels; c++) {
						const int channel = c % signalChannels;
						for (SampleIndex s = 0; s < remaining; s++) {
							target[c][s] +=
								signal[channel][(v.time + s) % signalLength] *
								gain * pan.volumes[c];
						}
					}
					v.time = (v.time + frames) % signal.size(); // progress voice and keep it in bounds

					if (v.loop) {
						return true; // we don't stop the voice when looping
					}
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
							fd.lowpassScratch[c]	= signal[c % signalChannels][v.time];
						}
						v.started = true;
					}

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

							if (signalLength <= nextIndex && !v.loop) {
								finished = true;
								break;
							}

							Sample mix = position - lastPosition;
							// mix = 0.5 * (1.0 - cos((mix) * 3.1416)); // cosine interpolation, introduces new harmonics somehow
							const Sample last = signal[channel][lastIndex % signalLength] * gain;
							const Sample next = signal[channel][nextIndex % signalLength] * gain;
							// linear resampling, sounds alright enough
							const Sample in = last + mix * (next - last);

							//	* super simple lowpass and highpass filter
							// just lerps with a previous value
							const Sample lpd = in + fd.lowpass * (fd.lowpassScratch[c] - in);
							fd.lowpassScratch[c] = lpd;

							const Sample hps = fd.highpassScratch[c];
							const Sample hpd = hps + fd.highpass * (in - hps);
							fd.highpassScratch[c] = hpd;

							target[c][s] += (lpd - hpd) * pan.volumes[c];
						}
					}
					position += speed; 					// step to next sample
					v.time = std::floor(position);		// split the signal in normal sample position
					fd.timeFract = position - v.time;	// and fractional time for the next block
					v.time = v.time % signalLength;		// set index back into bounds if we're looping
					return !finished;					// is only true when exceeding signalLength and not looping
				}
			});
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
