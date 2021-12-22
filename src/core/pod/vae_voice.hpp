#ifndef _VAE_VOICE
#define _VAE_VOICE

#include "../vae_types.hpp"
#include <limits>
#include "../vae_config.hpp"

namespace vae { namespace core {
	/**
	 * @brief Barebones voice. Enough to play back a non spatialized sound.
	 */
	struct Voice {
		bool spatialized : 1;	///< If the voice has spatialization data
		bool chainedEvents : 1;	///< If this voice triggers events after it stopped playing
		bool started : 1;		///< Whether the voice has started playing
		bool audible : 1;		///< Whether the voice was heard by any listener
		bool HRTF : 1;			///< If the voice should be rendered using hrtfs
		bool mixMatrix : 1;		//! TODO If the voice has a mix matrix
		bool filtered : 1;		//! TODO If the voice is filtered
		bool _placeholder : 1;
		BankHandle bank;							///< Which bank it belongs to
		SourceHandle source = InvalidSourceHandle;	///< If invalid, means voice is not playing.
		EventHandle event;							///< Which event triggered the voice to be played
		EventHandle eventInstance;					///< Not needed?
		EmitterHandle emitter;						//
		MixerHandle mixer;							///< Where the voice should mix to
		Sample gain = 1.0;							///< Volume of the voice
		SampleIndex time = 0;						///< Current time in samples
	};

	constexpr int _VAE_VOICE_SIZE = sizeof(Voice);

	struct VoiceFilered {
		Sample lowpassScratch[Config::MaxChannels];
		Sample highpassScratch[Config::MaxChannels];
		Sample lowpass = 0.0;	//< Lowpasses the signal as the value approaches 1
		Sample highpass = 0.0;	//< Highpasses the signal as the value approaches 1
		Sample speed = 1.0;		//< Playback speed, will alter pitch
	};

	constexpr int _VAE_VOICE_FILTERED_SIZE = sizeof(VoiceFilered);

	/**
	 * @brief VoicePanningInterpolation data.
	 * @details Extends Voice with channel volumes for interpolation.
	 */
	struct VoicePIP {
		/**
		 * @brief Current volume and phase for a single Listener
		 */
		struct Panning {
			Sample volumes[Config::MaxChannels];
			Sample phases[Config::MaxChannels];
		};
		/**
		 * @brief Each listener needs own panning data
		 */
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

	constexpr int _VAE_VOICE_PIP_SIZE = sizeof(VoicePIP);

} } // core::vae

#endif // _VAE_VOICE
