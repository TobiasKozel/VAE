#ifndef _VAE_CONFIG
#define _VAE_CONFIG

namespace vae {
	namespace Config {
		constexpr int SampleRate = 48000; // Internal Samplerate everything will run at
		constexpr unsigned char MaxChannels = 16;
		// DSP can only handle blocks smaller to save space
		constexpr unsigned int MaxBlock = 512;
		constexpr unsigned int DeviceMaxPeriods = 3;
	}
}

#endif // VAE_CONFIG
