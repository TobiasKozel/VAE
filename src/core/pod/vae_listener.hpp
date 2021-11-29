#ifndef _VAE_LISTENER
#define _VAE_LISTENER


#include "../vae_types.hpp"
#include <array>

namespace vae { namespace core {

	struct Listener {
		Vec3 postion;
		Vec3 orientation;
		ListenerHandle id = InvalidListenerHandle;
	};

	using Listeners = std::array<Listener, Config::MaxListeners>;

	constexpr int _VAE_LISTENER_SIZE = sizeof(Listener);
} } // vae::core

#endif // _VAE_LISTENER
