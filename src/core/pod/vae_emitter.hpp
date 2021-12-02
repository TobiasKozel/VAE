#ifndef _VAE_EMITTER
#define _VAE_EMITTER

#include "../vae_types.hpp"

namespace vae { namespace core {
	struct Emitter {
		Vec3 position;
		Vec3 direction;
		Sample spread;
		EventHandle event;
		BankHandle bank;
	};

	constexpr int _VAE_EMITTER_SIZE = sizeof(Emitter);
} } // vae::core

#endif // _VAE_EMITTER
