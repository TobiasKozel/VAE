#ifndef _VAE_CONFIG
#define _VAE_CONFIG

namespace vae {

	/**
	 * Contains some fundamental Configuration
	 */
	namespace Config {
		/**
		 * When a device is openeed. it will try to use this samplerate.
		 * If it doesn't support it, a resampler is used.
		 * This is efficient if most of the audio is in the preferred samplerate,
		 * since they don't need to be resampled.
		 */
		constexpr int PreferredSampleRate = 48000;

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
	}
}

#endif // VAE_CONFIG
