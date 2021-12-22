#ifndef _VAE_CONFIG
#define _VAE_CONFIG

#include "./vae_types.hpp"

namespace vae { namespace core {

	/**
	 * @brief Contains some fundamental Configuration needed at compile time
	 * Dynamic settings are contained in the EngineSettings struct
	 */
	namespace Config {
		/**
		 * Maximum channel count used to pre allocate buffers
		 */
		constexpr unsigned char MaxChannels = 2;

		/**
		 * Maximum block size.
		 * Used to preallocate buffers.
		 * Higher values need more memory might play better with instruction caches.
		 */
		constexpr unsigned int MaxBlock = 256;

		/**
		 * @brief How many Samples to prefetch for streaming sources
		 * TODO no streaming for now
		 */
		constexpr unsigned int StreamPrefetch = 1024 * 8;

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

#endif // VAE_CONFIG
