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
			for (auto& v : manager.voices) {
				if (v.source == InvalidSourceHandle) { continue; }			// not playing
				if (v.bank != bank.id) { continue; }						// wrong bank
				if (!v.flags[Voice::Flags::spatialized]) { continue; }		// not spatialized

				auto& source = bank.sources[v.source];
				auto& signal = source.signal;

				if (signal.size() == 0) {
					manager.stopVoice(v);
					continue; // no signal
				}

				VAE_ASSERT(signal.sampleRate == sampleRate)					// needs resampling

				const SampleIndex remaining = std::min(
					frames, SampleIndex(signal.size() - v.time
				));

				v.flags[Voice::Flags::started] = true;

				if (!spatial.hasEmitter(v.emitter)) { // emitter missing
					v.time += remaining; // progress time in voice
					if (remaining != frames) {
						manager.stopVoice(v); // end is reached
					}
					continue;
				}

				auto& mixer = bank.mixers[v.mixer];
				auto& target = mixer.buffer;
				auto& emitter = spatial.getEmitter(v.emitter);
				auto& listeners = spatial.getListeners();

				target.setValidSize(frames); // mark mixer as active

				const auto gain = v.gain * source.gain;

				switch (listeners[0].configuraion) {
				case Listener::Configuration::Headphones:
					// Each listener gets the sound mixed in from it's position
					for (auto& listener : listeners) {
						if (listener.id == InvalidListenerHandle) { continue; }
						Sample distance = glm::distance(listener.postion, emitter.position);
						distance = std::max(distance, Sample(1));
						distance = std::min(distance, Sample(1000000));
						distance = std::pow(distance / 1, -Sample(1));
						const auto attenuated = gain * distance;
						for (SampleIndex s = 0; s < remaining; s++) {
							const Sample sample = signal[0][v.time + s] * attenuated;
							target[0][s] += sample;
							target[1][s] += sample;
						}
					}
					break;
				default:
					break;
				}

				v.time += remaining; // progress time in voice
				if (remaining != frames) {
					manager.stopVoice(v); // end is reached
				}
			}
		}
	};

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
