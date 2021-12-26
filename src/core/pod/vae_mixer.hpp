#ifndef _VAE_MIXER
#define _VAE_MIXER

#include "../vae_types.hpp"
#include "./vae_effect.hpp"
#include "../vae_config.hpp"

#include <array>

namespace vae { namespace core {
	struct Mixer {
		/**
		 * @brief This is the master mixer for a bank
		 */
		static constexpr MixerHandle MasterMixerHandle = 0;
		AudioBuffer buffer; // not very POD
		Sample gain;
		MixerHandle parent;
		/**
		 * @brief The handle of the mixer the signal will be routed to.
		 * 0 is always the bank master and the default.
		 */
		MixerHandle id = InvalidMixerHandle;
		StackBuffer<Effect, StaticConfig::MaxMixerEffects> effects;
		NameString name; // name for debugging

		Mixer() {
			gain = 1.0;
			parent = MasterMixerHandle;
		}
	};

	constexpr int _VAE_MIXER_SIZE = sizeof(Mixer);
} } // vae::core

#endif // _VAE_MIXER
