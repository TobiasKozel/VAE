#ifndef _VAE_VOICE_PAN
#define _VAE_VOICE_PAN

#include "../vae_types.hpp"
#include "../vae_config.hpp"

namespace vae { namespace core {
	/**
	 * @brief Data to interpolate panning between blocks or do manual pan.
	 * @details Extends Voice with channel volumes for interpolation.
	 */
	struct VoicePan {
		Sample volumes[StaticConfig::MaxChannels];
		// StackBuffer<Sample, StaticConfig::MaxChannels> phases; // don't thing we need phase for now

		VoicePan() {
			for (int i = 0; i < StaticConfig::MaxChannels; i++) {
				volumes[i] = 1.0;
			}
		}
	};

} } // core::vae

#endif // _VAE_VOICE_PAN
