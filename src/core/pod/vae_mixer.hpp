#ifndef _VAE_MIXER
#define _VAE_MIXER

#include "../vae_types.hpp"
#include "./vae_effect.hpp"

#include <string>
#include <vector>

namespace vae { namespace core {
	struct Mixer {
		/**
		 * @brief This is the master mixer for a bank
		 */
		static constexpr MixerHandle MasterMixerHandle = 0;
		AudioBuffer buffer;
		Sample gain = 1.0;
		MixerHandle parent;
		/**
		 * @brief The handle of the mixer the signal will be routed to.
		 * 0 is always the master and the default.
		 * It's the only mixer shared across banks.
		 */
		MixerHandle id = InvalidMixerHandle;
		std::vector<Effect> effects;
		std::string name;
	};

	constexpr int _VAE_VECTOR_SIZE = sizeof(std::vector<Effect>);
	constexpr int _VAE_MIXER_SIZE = sizeof(Mixer);
} } // vae::core

#endif // _VAE_MIXER
