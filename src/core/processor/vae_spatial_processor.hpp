#ifndef _VAE_SPATIAL_PROCESSOR
#define _VAE_SPATIAL_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../pod/vae_listener.hpp"
#include "../vae_voice_manager.hpp"

namespace vae { namespace core {
	struct SpatialProcessor {
		/**
		 * @brief Process a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 * @param sampleRate
		 */
		static void mix(
			VoiceManger& manager, Bank& bank,
			Listeners& listeners,
			SampleIndex frames, Size sampleRate
		) {
			for (auto& v : manager.voices) {
				if (v.source == InvalidSourceHandle) { continue; }			// not playing
				if (v.bank != bank.id) { continue; }					// wrong bank
				if (!v.flags[Voice::Flags::spatialized]) { continue; }	// not spatialized

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;
				if (signal.size() == 0) { continue; }

				VAE_ASSERT(signal.sampleRate == sampleRate);

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;

				target.setValidSize(frames); // mark mixer as active

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				const auto gain = v.gain * source.gain;

				// Each listener gets the sound mixed in from it's position
				for (auto& listener : listeners) {
					if (listener.id == InvalidListenerHandle) { continue; }
					for (int c = 0; c < target.channels(); c++) {
						const int channel = c % signal.channels();
						for (SampleIndex s = 0; s < remaining; s++) {
							target[c][s] += signal[channel][v.time + s] * gain;
						}
					}
				}

				v.time += remaining; // progress time in voice

				if (remaining != frames) {
					// end is reached, voice needs to be stopped
					manager.stopVoice(v);
				}
			}
		}
	};

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
