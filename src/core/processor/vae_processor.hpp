#ifndef _VAE_PROCESSOR
#define _VAE_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
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
		 * @param sampleRate
		 */
		static void mix(
			VoiceManger& manager, Bank& bank,
			SampleIndex frames, Size sampleRate
		) {
			for (auto& v : manager.voices) {
				if (v.source == InvalidHandle) { continue; }
				if (v.bank != bank.id) { continue; }

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
					/**
					 * If the event triggers something on_end
					 * it needs to be added to the voicesFinished
					 * array in the voice manager.
					 * The update() function on the engine will handle it
					 */

					// TODO VAE PERF
					bool finished = false;
					for (auto& i : manager.voicesFinished) {
						if (i.source == InvalidHandle) {
							finished = true;
							i.event = v.event;
							i.eventInstance = v.eventInstance;
							i.mixer = v.mixer;
							i.emitter = v.emitter;
							i.bank = v.bank;

							// This is set last since it marks the
							// finished voice for other threads
							i.source = v.source;
							break;
						}
					}
					v.source = InvalidHandle; // Mark voice as free

					if (!finished) {
						// Failed to find a free spot in finished voices array
						// TODO handle this gracefully
						VAE_ASSERT(false)
					}
				}
			}
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
