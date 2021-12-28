#ifndef _VAE_EMITTER
#define _VAE_EMITTER

#include "../vae_types.hpp"
namespace vae { namespace core {
	/**
	 * @brief Allows placement of a Sound in 3D
	 * @details Also can trigger a event if a listener is closer than maxDist.
	 */
	struct Emitter {
		Vec3 position = { 0, 0, 0 };
		Real spread = 0.5;	///< The width of the source, 1 means all speaker will play
		Real maxDist = 100;	///< if event is valid, it will be emitted once the emitter is closer
		EventHandle event;		///< Will be triggerd
		BankHandle bank;		///< Needed to identify event
		bool autoplaying : 1;	///< whether the event was already triggered
	};

	constexpr int _VAE_EMITTER_SIZE = sizeof(Emitter);
} } // vae::core

#endif // _VAE_EMITTER
