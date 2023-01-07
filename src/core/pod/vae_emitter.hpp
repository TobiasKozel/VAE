#ifndef _VAE_EMITTER
#define _VAE_EMITTER

#include "../vae_types.hpp"
#include "../algo/vae_vec.hpp"
namespace vae { namespace core {
	/**
	 * @brief Allows placement of a Sound in 3D
	 * @details Also can trigger a event if a listener is closer than maxDist.
	 */
	struct Emitter {
		Vector3 position = { };
		Real separation = 1.0;					///< how much seperation between speaker will be when applying the panning law
		Real maxDist = 100;						///< if event is valid/positive, it will be emitted once the emitter is closer
		EventHandle event = InvalidEventHandle;	///< Will be triggerd when listener enters max dist
		BankHandle bank = InvalidBankHandle;	///< Needed to identify event when listener enters max dist
		bool started = false;					///< whether the event was already triggered
	};
} } // vae::core

#endif // _VAE_EMITTER
