#ifndef _VAE_PROCESSOR
#define _VAE_PROCESSOR

#include "../vae_types.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../voice/vae_voice_manager.hpp"

namespace vae { namespace core {

	struct Processor {
		static void mix(
			VoiceManger& manager, std::vector<Bank>& banks,
			BankHandle masterBank, SampleIndex frames
		) {
			auto& master = banks[masterBank].mixers[Mixer::MasterMixerHandle];

			for (auto& v : manager.voices) {
				if (v.source == InvalidHandle) { continue; }
				auto& bank = banks[v.bank];
				auto& source = bank.sources[v.source];
				auto& parent =
					(v.mixer == Mixer::MasterMixerHandle) ?
					master : bank.mixers[v.mixer];


				auto& signal = source.signal;
				auto& target = parent.buffer;

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
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
