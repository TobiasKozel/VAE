#ifndef _VAE_VOICE_PAN
#define _VAE_VOICE_PAN

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include <limits>

namespace vae { namespace core {
	/**
	 * @brief VoicePanningInterpolation data.
	 * @details Extends Voice with channel volumes for interpolation.
	 */
	struct VoicePan {
		StackBuffer<Sample, Config::MaxChannels> volumes;
		// StackBuffer<Sample, Config::MaxChannels> phases;
	};

	constexpr int _VAE_VOICE_PAN_PIP_SIZE = sizeof(VoicePan);

} } // core::vae

#endif // _VAE_VOICE_PAN
