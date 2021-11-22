#ifndef _VAE_BUS_EVENTS
#define _VAE_BUS_EVENTS

#include "../../external/tklb/src/types/TEventBus.hpp"

namespace vae { namespace core {
	/**
	 * List of all events
	 */
	enum BusEvents {
		EmitterDestroyed = 0, // provides a pointer to the emitter
		TOTAL_EVENTS
	};
	/**
	 * Type for the EventList above
	 */
	using EventBus = tklb::EventBus<BusEvents::TOTAL_EVENTS, BusEvents>;
} } // namespace vae::core

#endif // _VAE_BUS_EVENTS
