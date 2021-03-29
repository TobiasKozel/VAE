#ifndef VAE_API_DEVICE
#define VAE_API_DEVICE

namespace VAE {
	struct Device {
		unsigned int id;
		char name[255];
		unsigned int channels;
		unsigned int sampleRate;
	};
}

#endif
