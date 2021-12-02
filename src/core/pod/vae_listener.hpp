#ifndef _VAE_LISTENER
#define _VAE_LISTENER


#include "../vae_types.hpp"
#include <array>

namespace vae { namespace core {

	struct Listener {
		struct Configuration {
			enum {
				Mono = 0,
				Headphones,
				Stereo,
				HRTF,
				Quadrophonic,
				Suround
			};
		};
		Vec3 postion = { 0, 0, 0 };						// Absolute position
		Vec3 front = { 0, 0, 1 };						// used for non HRTF panning
		Vec3 up = { 0, 1, 0 };							// user for HRTF
		ListenerHandle id = InvalidListenerHandle;
		/**
		 * ! Listeners all mix to the same mixer channels and have the same configuration
		 * This might change, so that's why it's stored here
		 * Only the configuration for the first listeners is used
		 */
		Uchar configuraion = Configuration::Headphones;

	};

	using Listeners = std::array<Listener, Config::MaxListeners>;

	constexpr int _VAE_LISTENER_SIZE = sizeof(Listener);
} } // vae::core

#endif // _VAE_LISTENER
