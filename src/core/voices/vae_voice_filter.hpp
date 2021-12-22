#ifndef _VAE_VOICE_FILTER
#define _VAE_VOICE_FILTER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include <limits>

namespace vae { namespace core {
	/**
	 * @brief Additional data needed for filtered voices.
	 * Allows lowpass and highpass filtering as well as variable
	 * playback speeds.
	 */
	struct VoiceFilter {
		Sample lowpassScratch[Config::MaxChannels];		///< Last sample per channel for IIR filter
		Sample highpassScratch[Config::MaxChannels];	///< Last sample per channel for IIR filter
		Sample lowpass = 0.0;	///< Lowpasses the signal as the value approaches 1
		Sample highpass = 0.0;	///< Highpasses the signal as the value approaches 1
		Sample speed = 1.0;		///< Playback speed, will alter pitch
		Sample timeFract = 0.0;	///< Fractional time component for interpolation
	};

	constexpr int _VAE_VOICE_FILTER_FILTERED_SIZE = sizeof(VoiceFilter);

} } // core::vae

#endif // _VAE_VOICE_FILTER
