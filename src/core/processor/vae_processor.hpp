#ifndef _VAE_PROCESSOR
#define _VAE_PROCESSOR

#include "../vae_types.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../voice/vae_voice_manager.hpp"

namespace vae { namespace core {

	struct Processor {
		/**
		 * @brief Process a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 */
		static void mix(
			VoiceManger& manager, Bank& bank, SampleIndex frames
		) {
			/**
			 *
			 * Mix all the currently active voices first
			 *
			 */
			for (auto& v : manager.voices) {
				if (v.source == InvalidHandle) { continue; }
				if (v.bank != bank.id) { continue; }

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;
				if (signal.size() == 0) { continue; }

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;

				target.setValidSize(frames); // mark mixer as active

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				for (int c = 0; c < target.channels(); c++) {
					const int channel = c % signal.channels();
					for (SampleIndex s = 0; s < remaining; s++) {
						target[c][s] += signal[channel][v.time + s];
					}
				}

				v.time += remaining; // progress time in voice

				if (remaining != frames) {
					// end is reached
					auto& event = bank.events[v.event];
					if (event.on_end.empty()) {
						v.source = InvalidHandle; // Mark voice as free
						continue;
					}
					// If the event triggers something once a voice is done
					// it needs to be added to the voicesFinished array in the voice manager

					// TODO VAE PERF
					bool finished = false;
					for (auto& i : manager.voicesFinished) {
						if (i.source == InvalidHandle) {
							finished = true;
							i = std::move(v);
							break;
						}
					}
					v.source = InvalidHandle; // Mark voice as free

					if (!finished) {
						// Failed to find a free spot in finished voices array
						VAE_ASSERT(false)
					}
				}
			}

			// mix all non master channels
			// start from back since mixer can only write to mixer with
			// a lower id than themselves to avoid recursion
			for (int i = bank.mixers.size() - 1; 0 < i; i--) {
				auto& mixer = bank.mixers[i];
				// skip inactive mixers
				if (mixer.buffer.validSize() == 0) { continue; }

				auto& targetMixer = bank.mixers[mixer.parent];
				// mark mixer as active
				targetMixer.buffer.setValidSize(frames);
				targetMixer.buffer.add(mixer.buffer);

				// clear current mixer for next block
				mixer.buffer.set(0);
			}

			// Master mixer will be mixed to the final output in the engineand then cleared
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
