#ifndef _VAE_MIXER
#define _VAE_MIXER

#include "../vae_types.hpp"
#include "./vae_effect.hpp"
#include "../vae_config.hpp"

namespace vae { namespace core {
	/**
	 * @brief Mixer channel
	 *        TODO this object takes up a little much space
	 */
	struct Mixer {
		/**
		 * @brief This is the master mixer for a bank
		 */
		static constexpr MixerHandle MasterMixerHandle = 0;
		Sample gain = 1.0;
		MixerHandle parent = MasterMixerHandle;
		/**
		 * @brief The handle of the mixer the signal will be routed to.
		 * 0 is always the bank master and the default.
		 */
		MixerHandle id = InvalidMixerHandle;
		Effect effects[StaticConfig::MaxMixerEffects];
		ScratchBuffer buffer; // not very POD
		NameString name; // name for debugging
	};
} } // vae::core

#endif // _VAE_MIXER
