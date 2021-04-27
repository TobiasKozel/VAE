#ifndef VAE_IMPL_CONFIG
#define VAE_IMPL_CONFIG

namespace VAE {
	namespace Config {
		typedef float Sample;
		constexpr int SampleRate = 48000; // Internal Samplerate everything will run at
		constexpr unsigned char MaxChannels = 16;
		// DSP can only handle blocks smaller to save space
		constexpr unsigned int MaxBlock = 512;
		constexpr unsigned int DeviceMaxPeriods = 3;
	}
}

#endif // VAE_IMPL_CONFIG
