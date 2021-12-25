#ifndef _VAE_CONFIG
#define _VAE_CONFIG

#include "../../include/vae/vae.hpp"

namespace vae { namespace Config {
	// Most of the settings are in the public header since they are useful to know outside the engine
	// really private stuff can go here
} }

/**
 * @brief Forward internal max channels to tklb for tklb::AudioBuffer
 */
#define TKLB_MAXCHANNELS vae::StaticConfig::MaxChannels

#endif // VAE_CONFIG
