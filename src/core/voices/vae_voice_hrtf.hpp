#ifndef _VAE_VOICE_HRTF
#define _VAE_VOICE_HRTF

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include <limits>

namespace vae { namespace core {
	/**
	 * @brief Data needed to process HRTFs
	 */
	struct VoiceHRTF {
		SampleIndex convolutionIndex;
		AudioBuffer convolutionBuffer;
	};

	constexpr int _VAE_VOICE_HRTF_SIZE = sizeof(VoiceHRTF);

} } // core::vae

#endif // _VAE_VOICE_HRTF
