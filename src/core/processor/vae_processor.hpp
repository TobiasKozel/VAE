#ifndef _VAE_PROCESSOR
#define _VAE_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../vae_voice_manager.hpp"

namespace vae { namespace core {

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

				VAE_ASSERT(signal.sampleRate == sampleRate);

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;

				target.setValidSize(frames); // mark mixer as active


				const auto gain = v.gain * source.gain;

				bool finished = false;

				if (v.filtered) {
					auto& fd = manager.getVoiceFilter(index);

					if (v.time == 0) {
						for (int c = 0; c < Config::MaxChannels; c++) {
							fd.highpassScratch[c]	= 0;
							fd.lowpassScratch[c]	= signal[c % signal.channels()][0];
						}
					}

					const Size countIn = signal.size() - (v.time * fd.speed); // max samples we can read

					for (int c = 0; c < target.channels(); c++) {
						const int channel = c % signal.channels();
						for (SampleIndex s = 0; s < frames; s++) {
							//					* LERP
							const Sample position = (v.time + s) * fd.speed;
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
							const Sample in = last + mix * (next - last);

							//	* one pole highpass and lowpass filter

							const Sample lpd = in + fd.lowpass * (fd.lowpassScratch[c] - in);
							fd.lowpassScratch[c] = lpd;

							const Sample hps = fd.highpassScratch[c];
							const Sample hpd = hps + fd.highpass * (in - hps);
							fd.highpassScratch[c] = hpd;

							target[c][s] += (lpd - hpd);
						}
					}
					v.time += frames;
				} else {
					const SampleIndex remaining = std::min(
						frames, SampleIndex(signal.size() - v.time
					));
					for (int c = 0; c < target.channels(); c++) {
						const int channel = c % signal.channels();
						for (SampleIndex s = 0; s < remaining; s++) {
							target[c][s] += signal[channel][v.time + s] * gain;
						}
					}
					v.time += remaining; // progress time in voice
					finished = remaining == frames;
				}


				v.started = true;
				v.audible = true;

				return !finished;
			});
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
