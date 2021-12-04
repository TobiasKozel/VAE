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
			manager.forEachVoice([&](Voice& v, Size vi) {
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

				auto& lastPip = manager.getVoicePIP(vi);
				VoicePIP currentPip;

				target.setValidSize(frames); // mark mixer as active

				const auto gain = v.gain * source.gain;
				const Sample step = Sample(1) / Sample(frames);

				spatial.forEachListener([&](Listener& l, ListenerHandle li) {
					// Each listener gets the sound mixed in from it's position
					// ! this means using different configurations doesn't work !
					switch (l.configuraion) {
					case Listener::Configuration::Headphones:
						auto& currentVolumes = currentPip.listeners[li].volumes;
						auto& lastVolumes = lastPip.listeners[li].volumes;

						Vec3 direction = emitter.position - l.postion;
						Sample distanceAttenuated = glm::length(direction);
						direction /= distanceAttenuated;
						distanceAttenuated = std::max(distanceAttenuated, Sample(1));
						distanceAttenuated = std::min(distanceAttenuated, Sample(1000000));
						distanceAttenuated = Sample(std::pow(distanceAttenuated / Sample(1.0), -Sample(1)));
						distanceAttenuated *= gain;

						if (distanceAttenuated < 0.001) { return; } // ! inaudible

						Sample left =  glm::dot(l.front, { -1.f, 0.f, 0.f });
						Sample right = glm::dot(l.front, {  1.f, 0.f, 0.f });

						currentVolumes[0] = distanceAttenuated * left;
						currentVolumes[1] = distanceAttenuated * right;

						if (v.time == 0) {
							lastVolumes[0] = currentVolumes[0];
							lastVolumes[1] = currentVolumes[1];
						}

						for (SampleIndex s = 0; s < remaining; s++) {
							const Sample sample = signal[0][v.time + s];
							target[0][s] += sample * tklb::lerp(lastVolumes[0], currentVolumes[0], s * step);
							target[1][s] += sample * tklb::lerp(lastVolumes[1], currentVolumes[1], s * step);
						}
						return;
					}
				});

				lastPip = std::move(currentPip);

				v.time += remaining; // progress time in voice
				return remaining == frames;
			});
		}
	};

} } // core::vae

#endif // _VAE_SPATIAL_PROCESSOR
