#ifndef _VAE_MIXER_PROCESSOR
#define _VAE_MIXER_PROCESSOR

#include "../vae_types.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_mixer.hpp"
#include "../voice/vae_voice_manager.hpp"

namespace vae { namespace core {

	struct MixerProcessor {
		/**
		 * @brief Process the mixers for a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 */
		static void mix(
			VoiceManger& manager, Bank& bank, SampleIndex frames
		) {
			/**
			 * mix all non master mixers
			 * start from back since mixer can only write to mixer with
			 * a lower id than themselves to avoid recursion
			 */
			for (int i = bank.mixers.size() - 1; 0 < i; i--) {
				auto& sourceMixer = bank.mixers[i];
				// skip inactive mixers
				if (sourceMixer.buffer.validSize() == 0) { continue; }

				// TODO effects processing

				// Apply mixer volume
				// TODO PERF VAE might be better to apply gain and mix in one go
				sourceMixer.buffer.multiply(sourceMixer.gain);
				auto& targetMixer = bank.mixers[sourceMixer.parent];

				// mark mixer as active
				targetMixer.buffer.setValidSize(frames);

				targetMixer.buffer.add(sourceMixer.buffer);

				// clear current mixer for next block
				sourceMixer.buffer.set(0);
			}

			// Apply gain on master as well
			// TODO effects processing
			auto& masterMixer = bank.mixers[Mixer::MasterMixerHandle];
			masterMixer.buffer.multiply(masterMixer.gain);
			// Master mixer will be mixed to the final output in the engineand then cleared
		}
	};

} } // vae::core

#endif // _VAE_MIXER_PROCESSOR
