#ifndef _VAE_EVENT
#define _VAE_EVENT

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "./vae_mixer.hpp"

#include <array>
#include <bitset>

namespace vae { namespace core {
	struct Event {
		struct Flags {
			enum {
				start = 0,		// Starts a source
				stop,			// Stops a source
				emit,			// Emits an event to the EventCallback defined in the engine config
				force_mixer,	// Prevents overriding the mixer from chained events or fireEvent
				loop,			// TODO sound should loop
				HRTF,			// TODO If the listener has hrtfs set up it will be rendered using it
				FLAG_COUNT
			};
		};
		std::bitset<Flags::FLAG_COUNT> flags;						// Contains flags from above
		EventHandle id = InvalidEventHandle;								// Own id
		SourceHandle source = InvalidSourceHandle;						// Handle to a source
		Sample gain = 1.0;											// Volume applied to triggered voice
		std::array<EventHandle, Config::MaxChainedEvents> on_start;	// Events called when the source starts playing
		std::array<EventHandle, Config::MaxChainedEvents> on_end;	// Events fired once the source is finished, not called when there's no source
		MixerHandle mixer = Mixer::MasterMixerHandle;				// Mixer the source gets written to
		NameString name;											// Name for debugging

		/**
		 * TODO this isn't exactly pod style but the arrays need to be
		 *  initialized properly since 0 is a valid event handle
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
