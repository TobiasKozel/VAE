#ifndef _VAE_CONFIG
#define _VAE_CONFIG

namespace vae { namespace core {

	/**
	 * Contains some fundamental Configuration needed at compile time
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
		constexpr size_t MaxListeners = 4;

		/**
		 * @brief How many effects a mixer channel can process
		 */
		constexpr size_t MaxMixerEffects = 4;

		/**
		 * @brief How many chained events can fit in on_start and on_end
		 */
		constexpr size_t MaxChainedEvents = 4;
	}
} }

#endif // VAE_CONFIG
