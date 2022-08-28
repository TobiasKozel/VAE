#ifndef _VAE_VOICE_HRTF
#define _VAE_VOICE_HRTF

#include "../vae_types.hpp"
#include "../vae_config.hpp"

namespace vae { namespace core {
	/**
	 * @brief Data needed to process HRTFs
	 */
	struct VoiceHRTF {
		ScratchBuffer convolutionBuffer;	///< Temporary buffer for the convolution
		Size convolutionIndex;				///< position in convolution buffer
	};

	constexpr int _VAE_VOICE_HRTF_SIZE = sizeof(VoiceHRTF);

} } // core::vae

#endif // _VAE_VOICE_HRTF
