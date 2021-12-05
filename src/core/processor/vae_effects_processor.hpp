#ifndef _VAE_EFFECTS_PROCESSOR
#define _VAE_EFFECTS_PROCESSOR

#include "../pod/vae_effect.hpp"
#include "../pod/vae_mixer.hpp"

namespace vae { namespace core {
	struct EffectsProcessor {
		static void mix(Effect& effect, AudioBuffer& buffer) {
			// TODO processing
		}
	}; // EffectsProcessor
} } // vae::core

#endif // _VAE_EFFECTS_PROCESSOR
