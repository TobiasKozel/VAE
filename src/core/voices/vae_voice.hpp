#ifndef _VAE_VOICE
#define _VAE_VOICE

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "vae/vae.hpp"
#include <limits>

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
		bool spatialized : 1;	///< If the voice has spatialization data
		bool chainedEvents : 1;	///< If this voice triggers events after it stopped playing
		bool started : 1;		///< Whether the voice has started playing
		bool audible : 1;		///< Whether the voice was heard by any listener
		bool HRTF : 1;			///< If the voice should be rendered using hrtfs
		bool mixMatrix : 1;		//! TODO If the voice has a mix matrix
		bool filtered : 1;		//! TODO If the voice is filtered
		bool critical : 1;		///< Voice can't be killed in favor of new voice
		BankHandle bank;							///< Which bank it belongs to
		SourceHandle source = InvalidSourceHandle;	///< If invalid, means voice is not playing.
		EventHandle event;							///< Which event triggered the voice to be played
		EmitterHandle emitter;						///< Emitter used to control voice properties
		MixerHandle mixer;							///< Where the voice should mix to
		ListenerHandle listener;					///< If it's spatialized it's rendered for this listener
		Sample gain = 1.0;							///< Volume of the voice
		SampleIndex time = 0;						///< Current time in samples
	};

	constexpr int _VAE_VOICE_SIZE = sizeof(Voice);
} } // core::vae

#endif // _VAE_VOICE
