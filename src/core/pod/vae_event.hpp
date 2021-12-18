#ifndef _VAE_EVENT
#define _VAE_EVENT

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "./vae_mixer.hpp"



namespace vae { namespace core {
	struct Event {
		bool start 	: 1;		// Starts a source
		bool stop : 1;			// Stops a source
		bool emit : 1;			// Emits an event to the EventCallback defined in the engine config
		bool force_mixer : 1;	// Prevents overriding the mixer from chained events or fireEvent
		bool loop : 1;			// TODO sound should loop
		bool HRTF : 1;			// TODO If the listener has hrtfs set up it will be rendered using it
		bool random : 1;		// triggers one random on_start event
		bool _placholder : 1;

		MixerHandle mixer = Mixer::MasterMixerHandle;				// Mixer the source gets written to
		EventHandle id = InvalidEventHandle;						// Own id
		SourceHandle source = InvalidSourceHandle;					// Handle to a source
		Sample gain = 1.0;											// Volume applied to triggered voice
		StackBuffer<EventHandle, Config::MaxChainedEvents> on_start;// Events called when the source starts playing
		StackBuffer<EventHandle, Config::MaxChainedEvents> on_end;	// Events fired once the source is finished, not called when there's no source
		NameString name;											// Name for debugging

		/**
		 * TODO this isn't exactly pod style but the arrays need to be
		 * initialized properly since 0 is a valid event handle
		 */
		Event() {
			for (size_t i = 0; i < Config::MaxChainedEvents; i++) {
				on_start[i] = InvalidEventHandle;
				on_end[i] = InvalidEventHandle;
			}
		}
	};

	constexpr int _VAE_EVENT_SIZE = sizeof(Event);
} } // namespace vae::vore

#endif // _VAE_EVENT
