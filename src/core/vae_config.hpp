#ifndef _VAE_CONFIG
#define _VAE_CONFIG

#include "../../include/vae/vae.hpp"

namespace vae { namespace core {

	/**
	 * @brief Contains some fundamental Configuration needed at compile time
	 * Dynamic settings are contained in the EngineSettings struct
	 */
	namespace Config {
		/**
		 * @brief Maximum channel count used to pre allocate buffers
		 */
		constexpr unsigned char MaxChannels = 2;

		/**
		 * @brief Maximum block size
		 * Used to preallocate buffers.
		 * Higher values need more memory might play better with instruction caches
		 * but uses more memory.
		 */
		constexpr Size MaxBlock = 512;

		/**
		 * @brief How many Samples to prefetch for streaming sources
		 * TODO no streaming for now
		 */
		constexpr Size StreamPrefetch = 1024 * 8;

		/**
		 * @brief How many listeners can observe 3D voices
		 */
		constexpr Size MaxListeners = 4;

		/**
		 * @brief How many effects a mixer channel can process
		 */
		constexpr Size MaxMixerEffects = 4;

		/**
		 * @brief How many chained events can fit in on_start and on_end
		 */
		constexpr Size MaxChainedEvents = 4;

		/**
		 * @brief Minimum volume before sounds will skip rendering
		 */
		constexpr Sample MinVolume  = 0.01;
	}
} }

#define TKLB_MAXCHANNELS vae::core::Config::MaxChannels

#endif // VAE_CONFIG
