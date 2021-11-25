#ifndef _VAE_VOICE
#define _VAE_VOICE

#include "../vae_types.hpp"

namespace vae { namespace core {
	struct Voice {
		SourceHandle source = InvalidHandle;	// If invalid, means voice is not playing.
		EventHandle event;
		EventHandle eventInstance;
		MixerHandle mixer;
		EmitterHandle emitter;
		BankHandle bank;
		SampleIndex time = 0;
		Time timeFract = 0.0;
	};
} } // core::vae

#endif // _vAE_VOICE
