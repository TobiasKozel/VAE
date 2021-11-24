#ifndef _VAE_MIXER
#define _VAE_MIXER

#include "../vae_types.hpp"
#include "./vae_effect.hpp"

#include <string>
#include <vector>

namespace vae { namespace core {
	struct Mixer {
		MixerHandle id = InvalidHandle;
		std::string name;
		std::vector<Effect> effects;
		/**
		 * @brief The handle of the mixer the signal will be routed to
		 * 0 is always the master and the default.
		 * It's the only mixer shared across banks.
		 */
		MixerHandle parent;
	};
} } // vae::core

#endif // _VAE_MIXER
