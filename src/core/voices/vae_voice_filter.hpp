#ifndef _VAE_VOICE_FILTER
#define _VAE_VOICE_FILTER

#include "../vae_types.hpp"
#include "../vae_config.hpp"

namespace vae { namespace core {
	/**
	 * @brief Additional data needed for filtered voices.
	 * Allows lowpass and highpass filtering as well as variable
	 * playback speeds.
	 */
	struct FilteredVoice {
		Real lowpass = 0.0;	///< Lowpasses the signal as the value approaches 1
		Real highpass = 0.0;	///< Highpasses the signal as the value approaches 1
		Real speed = 1.0;		///< Playback speed, will alter pitch
		Real timeFract = 0.0;	///< Fractional time component for interpolation, [0.0-1.0]
		Sample lowpassScratch[StaticConfig::MaxChannels];		///< Last sample per channel for IIR filter
		Sample highpassScratch[StaticConfig::MaxChannels];	///< Last sample per channel for IIR filter

		FilteredVoice() {
			for (Size i = 0; i < StaticConfig::MaxChannels; i++) {
				lowpassScratch[i]  = 0;
				highpassScratch[i] = 0;
			}
		}
	};

} } // core::vae

#endif // _VAE_VOICE_FILTER
