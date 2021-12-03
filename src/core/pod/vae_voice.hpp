#ifndef _VAE_VOICE
#define _VAE_VOICE

#include "../vae_types.hpp"
#include <bitset>
#include <limits>
namespace vae { namespace core {
	/**
	 * @brief Barebones voice.
	 */
	struct Voice {
		struct Flags {
			enum {
				chainedEvents = 0,	// If this voice triggers events after it stopped playing
				started,
				spatialized,		// If the voice has spatialization data
				mixMatrix,			// If the voice has a mix matrix
				filtered,			// If the voice is filtered
				FLAG_COUNT
			};
		};
		SourceHandle source = InvalidSourceHandle;	// If invalid, means voice is not playing.
		EventHandle event;							// Which event triggered the voice to be played
		EventHandle eventInstance;					// Not needed?
		EmitterHandle emitter;						//
		MixerHandle mixer;							// Where the voice should mix to
		BankHandle bank;							// Which bank it belongs to
		Sample gain = 1.0;							// Volume of the voice
		SampleIndex time = 0;						// Current time in samples
		std::bitset<Flags::FLAG_COUNT> flags;
	};

	constexpr int _VAE_VOICE_SIZE = sizeof(Voice);

	/**
	 * @brief VoicePanningInterpolation data
	 *
	 */
	struct VoicePIP {
		struct Panning {
			Sample volumes[Config::MaxChannels];
			Sample phases[Config::MaxChannels];
		};
		Panning listeners[Config::MaxListeners];

		VoicePIP() {
			for (auto& l : listeners) {
				for (int i = 0; i < Config::MaxChannels; i++) {
					l.volumes[i] = std::numeric_limits<Sample>::min();
					l.phases[i] = std::numeric_limits<Sample>::min();
				}
			}
		}
	};

} } // core::vae

#endif // _VAE_VOICE
