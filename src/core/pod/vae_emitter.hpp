#ifndef _VAE_EMITTER
#define _VAE_EMITTER

#include "../vae_types.hpp"

namespace vae { namespace core {
	struct Emitter {
		Vec3 position = { 0, 0, 0 };
		Vec3 direction = { 0, 0, 0 };
		Sample spread = 0.5;
		EventHandle event;
		BankHandle bank;
	};

	constexpr int _VAE_EMITTER_SIZE = sizeof(Emitter);
} } // vae::core

#endif // _VAE_EMITTER
