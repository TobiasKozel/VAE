#ifndef _VAE_EMITTER_BASE
#define _VAE_EMITTER_BASE

#include <bitset>

#include "../source/vae_source_base.hpp"
#include "../vae_types.hpp"

namespace vae { namespace core {

	class Emitter {
	public:
		/**
		 * Shares current state and configuration to save some space
		 */
		enum State {
			playing = 0,	// Set once the Emitter has a any voice type assigned
			virt,			// Set once the Emitter has a virtual voice assigned
			canvirt,		// Whether the assigned voice can change to virtual
			killable,		// Whether the assigned voice can be taken away
			loop,			// Will not stop playing when the end of the source is reached
			binaural,		// Only applies to Spatial sources
			doppler,		// Only applies to Spatial sources
			delay,			// Only applies to Spatial sources
			STATE_COUNT
		};
	private:
		// Source* mSource = nullptr;					// Pointer to object generating the sound
		Sample mGain = 1.0;							// volume the source signal is multiplied by
		float mSpeed = 1.0;							// Playback speed
		std::bitset<State::STATE_COUNT> mState;		// Current state and configuration of the emitter
	public:
	};

} } // namespace vae::core

#endif // _VAE_EMITTER_BASE
