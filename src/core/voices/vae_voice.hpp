#ifndef _VAE_VOICE
#define _VAE_VOICE

#include "../vae_types.hpp"
#include "../vae_config.hpp"

namespace vae { namespace core {
	/**
	 * @brief Barebones voice.
	 * Enough to play back a non spatialized and non filtered sound.
	 * Other structs extend this and depend on it
	 * @see VoicePan
	 * @see VoiceHRTF
	 * @see VoiceFilter
	 */
	struct Voice {
		bool spatialized VAE_SMALL(1);		///< If the voice has spatialization data
		bool chainedEvents VAE_SMALL(1);	///< If this voice triggers events after it stopped playing
		bool started VAE_SMALL(1);			///< Whether the voice has started playing
		bool audible VAE_SMALL(1);			///< Whether the voice was heard by any listener
		bool HRTF VAE_SMALL(1);			///< If the voice should be rendered using hrtfs
		bool loop VAE_SMALL(1);			///< Voice will loop until killed
		bool filtered VAE_SMALL(1);		///< This will enable high/lowpass filters and variable speed playback. Gets turned on when signal does not match EngineStaticConfig::internalSampleRate
		bool critical VAE_SMALL(1);		///< Voice can't be killed in favor of new voice
		bool attenuate VAE_SMALL(1);		///< Whether distance affects volume
		BankHandle bank;							///< Which bank it belongs to
		SourceHandle source = InvalidSourceHandle;	///< If invalid, means voice is not playing.
		EventHandle event;							///< Which event triggered the voice to be played
		EmitterHandle emitter;						///< Emitter used to control voice properties
		MixerHandle mixer;							///< Where the voice should mix to
		ListenerHandle listener;					///< If it's spatialized it's rendered for this listener
		Sample gain = 1.0;							///< Volume of the voice
		SampleIndex time = 0;						///< Current time in samples

		Voice() {
			spatialized = false;
			chainedEvents = false;
			started = false;
			audible = false;
			HRTF = false;
			loop = false;
			filtered = false;
			critical = false;
			attenuate = false;
		}
	};
} } // core::vae

#endif // _VAE_VOICE
