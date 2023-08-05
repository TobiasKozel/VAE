#ifndef _VAE_LISTENER
#define _VAE_LISTENER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../algo/vae_vec.hpp"

namespace vae { namespace core {
	struct Listener {
		ListenerProperties properties;
		ListenerHandle id = InvalidListenerHandle;
		/**
		 * ! Listeners all mix to the same mixer channels and have the same configuration
		 * This might change, so that's why it's stored here
		 * Only the configuration for the first listeners is used
		 */
		SpeakerConfiguration configuration VAE_SMALL(3);
	};

	using Listeners = Listener[StaticConfig::MaxListeners];

	constexpr int _VAE_LISTENER_SIZE = sizeof(Listener);
} } // vae::core

#endif // _VAE_LISTENER
