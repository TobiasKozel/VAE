#ifndef _VAE_VOICE
#define _VAE_VOICE

#include "../vae_types.hpp"
#include <bitset>

namespace vae { namespace core {
	struct Voice {
		struct Flags {
			enum {
				chainedEvents = 0,	// If this voice triggers events after it stopped playing
				spatialized,		// If the voice has spatialization data
				mixMatrix,			// If the voice has a mix matrix
				filtered,			// If the voice is filtered
				FLAG_COUNT
			};
		};
		SourceHandle source = InvalidHandle;	// If invalid, means voice is not playing.
		EventHandle event;						// Which event triggered the voice to be played
		EventHandle eventInstance;				//
		EmitterHandle emitter;					//
		MixerHandle mixer;						// Where the voice should mix to
		BankHandle bank;						// Which bank it belongs to
		Sample gain = 1.0;						// Volume of the voice
		SampleIndex time = 0;					// Current time in samples
		Time timeFract = 0.0;					// Current time in seconds
		std::bitset<Flags::FLAG_COUNT> flags;
	};

	constexpr int _VAE_VOICE_SIZE = sizeof(Voice);
} } // core::vae

#endif // _VAE_VOICE
