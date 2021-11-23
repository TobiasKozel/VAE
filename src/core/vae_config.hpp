#ifndef _VAE_CONFIG
#define _VAE_CONFIG

namespace vae {

	/**
	 * Contains some fundamental Configuration needed at compile time
	 * Dynamic settings are contained in the EngineSettings struct
	 */
	namespace Config {
		/**
		 * Maximum channel count used to pre allocate buffers
		 */
		constexpr unsigned char MaxChannels = 16;

		/**
		 * Maximum block size.
		 * Used to preallocate buffers.
		 * Higher values increase latency but might play better with CPU caches.
		 */
		constexpr unsigned int MaxBlock = 512;

		/**
		 * When not running in synchronous mode, this represents the
		 * number of blocks processed ahead for swapping.
		 * Increases latency but reduces chances of underruns
		 * since it's more forgiving to the scheduler.
		 */
		constexpr unsigned int DeviceMaxPeriods = 3;

		/**
		 * @brief How many Samples to prefetch for streaming sources
		 */
		constexpr unsigned int StreamPrefetch = 1024 * 8;
	}
}

#endif // VAE_CONFIG
