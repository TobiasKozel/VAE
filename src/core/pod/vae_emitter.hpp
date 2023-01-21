#ifndef _VAE_EMITTER
#define _VAE_EMITTER

#include "../vae_types.hpp"
#include "../algo/vae_vec.hpp"
namespace vae { namespace core {
	/**
	 * @brief Allows placement of a Sound in 3D
	 * @details Also can trigger a event if a listener is closer than maxDist.
	 * TODO this needs to be part of the public API
	 */
	struct Emitter {
		Vector3 position = { };					///< Every emitter has a position, even if it's not spatial.
		Real separation = 1.0;					///< how much seperation between speaker will be when applying the panning law					///< whether the event was already triggered
	};
} } // vae::core

#endif // _VAE_EMITTER
