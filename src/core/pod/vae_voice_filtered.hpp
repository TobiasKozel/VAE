#ifndef _VAE_VOICE_FILTERED
#define _VAE_VOICE_FILTERED

#include "../vae_types.hpp"

namespace vae { namespace core {
	struct VoiceFiltered {
		Time timeFract = 0.0;					// Current time in seconds
	};
} } // core::vae

#endif // _VAE_VOICE_FILTERED
