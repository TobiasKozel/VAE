#ifndef _VAE_EVENT
#define _VAE_EVENT

#include "../vae_types.hpp"
#include "./vae_mixer.hpp"

#include <string>
#include <vector>

namespace vae { namespace core {
	struct Event {
		enum class EventType {
			undefined = 0,
			start,				// Starts a source
			stop,				// Stops a source
			emit				// Emits an event to the EventCallback defined in the engine config
		};
		EventHandle id = InvalidHandle;
		MixerHandle mixer = Mixer::MasterMixerHandle;	// Mixer the source gets written to
		bool force_mixer = false;						// Prevents overriding the mixer from chained events or fireEvent
		std::string name;								// Name for debugging
		std::vector<EventHandle> on_start;				// Events called when the source starts playing
		std::vector<EventHandle> on_end;				// Events fired once the source is finished, not called when there's no source
		SourceHandle source = InvalidHandle;			// Handle to a source
		EventType type;									// Defines what the event does
	};

	constexpr int _VAE_EVENT_SIZE = sizeof(Event);
} } // namespace vae::vore

#endif // _VAE_EVENT
