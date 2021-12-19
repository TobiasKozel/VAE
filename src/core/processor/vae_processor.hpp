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

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				const auto gain = v.gain * source.gain;

				for (int c = 0; c < target.channels(); c++) {
					const int channel = c % signal.channels();
					for (SampleIndex s = 0; s < remaining; s++) {
						target[c][s] += signal[channel][v.time + s] * gain;
					}
				}

				v.started = true;
				v.audible = true;

				v.time += remaining; // progress time in voice
				return remaining != frames;
			});
		}
	};

} } // vae::core

#endif // _VAE_PROCESSOR
