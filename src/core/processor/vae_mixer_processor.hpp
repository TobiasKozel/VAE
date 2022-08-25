#ifndef _VAE_MIXER_PROCESSOR
#define _VAE_MIXER_PROCESSOR

#include "../vae_types.hpp"
#include "../pod/vae_bank.hpp"
#include "../pod/vae_mixer.hpp"
#include "../vae_voice_manager.hpp"
#include "./vae_effects_processor.hpp"

namespace vae { namespace core {

	class MixerProcessor {
		EffectsProcessor mEffectsProcessor;
	public:
		void init() {
			mEffectsProcessor.init();
		}
		/**
		 * @brief Process the mixers for a single bank
		 *
		 * @param manager
		 * @param banks
		 * @param frames
		 */
		void mix(
			VoiceManger& manager, Bank& bank, SampleIndex frames
		) {
			TKLB_PROFILER_SCOPE_NAMED("Bank Mixer Processor")
			TKLB_ASSERT(!bank.mixers.empty()) // can't happen

			/**
			 * mix all non master mixers
			 * start from back since mixer can only write to mixer with
			 * a lower id than themselves to avoid recursion
			 */
			for (Uint i = Uint(bank.mixers.size()) - 1; 0 < i; i--) {
				TKLB_PROFILER_SCOPE_NAMED("Process Mixer Channel")
				auto& sourceMixer = bank.mixers[i];
				// skip inactive mixers
				if (sourceMixer.buffer.validSize() == 0) { continue; }

				for (auto& effect : sourceMixer.effects) {
					mEffectsProcessor.mix(effect, sourceMixer.buffer);
				}

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

			auto& masterMixer = bank.mixers[Mixer::MasterMixerHandle];
			for (auto& effect : masterMixer.effects) {
				mEffectsProcessor.mix(effect, masterMixer.buffer);
			}
			// Apply gain on master as well
			masterMixer.buffer.multiply(masterMixer.gain);
			// Master mixer will be mixed to the final output in the engine and then cleared
		}
	};

} } // vae::core

#endif // _VAE_MIXER_PROCESSOR
