#ifndef _VAE_EMITTER
#define _VAE_EMITTER

#include "../vae_types.hpp"
namespace vae { namespace core {
	struct Emitter {
		Vec3 position = { 0, 0, 0 };
		Sample spread = 0.5;
		Sample maxDist = 100; // if event is valid, it will be emitted once the emitter is closer
		EventHandle event;
		BankHandle bank;
		bool autoplaying : 1; // whether the event was already triggered
	};

	constexpr int _VAE_EMITTER_SIZE = sizeof(Emitter);
} } // vae::core

#endif // _VAE_EMITTER
