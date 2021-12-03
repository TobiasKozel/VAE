#ifndef _VAE_SPATIAL_PROCESSOR
#define _VAE_SPATIAL_PROCESSOR

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_voice.hpp"
#include "../vae_voice_manager.hpp"
#include "../vae_spatial_manager.hpp"

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
			SpatialManager& spatial,
			SampleIndex frames, Size sampleRate
		) {
			manager.forEachVoice([&](Voice& v) {
				if (v.bank != bank.id) { return true; }						// wrong bank
				if (!v.flags[Voice::Flags::spatialized]) { return true; }	// not spatialized

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				if (signal.size() == 0) { return false; }

				VAE_ASSERT(signal.sampleRate == sampleRate)					// needs resampling

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				v.flags[Voice::Flags::started] = true;

				if (!spatial.hasEmitter(v.emitter)) { // emitter missing
					v.time += remaining; // progress time in voice
					return remaining == frames;
				}

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;
				auto& emitter = spatial.getEmitter(v.emitter);

				target.setValidSize(frames); // mark mixer as active

				const auto gain = v.gain * source.gain;

				spatial.forEachListener([&](Listener& l) {
					switch (l.configuraion) {
					case Listener::Configuration::Headphones:
						// Each listener gets the sound mixed in from it's position
						Sample distance = glm::distance(l.postion, emitter.position);
						distance = std::max(distance, Sample(1));
						distance = std::min(distance, Sample(1000000));
						distance = std::pow(distance / 1, -Sample(1));
						distance *= gain;
						for (SampleIndex s = 0; s < remaining; s++) {
							const Sample sample = signal[0][v.time + s] * distance;
							target[0][s] += sample;
							target[1][s] += sample;
						}
						break;
					}
				});

				v.time += remaining; // progress time in voice
				return remaining == frames;
			});
		}
	};

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
