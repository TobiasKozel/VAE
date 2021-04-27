#ifndef VAE_API_DEVICE
#define VAE_API_DEVICE

namespace VAE {
	struct DeviceInfo {
		/**
		 * 0 for default device.
		 * Negative values for invalid device.
		 */
		int id;
		char name[255];
		unsigned int channelsIn;
		unsigned int channelsOut;
		unsigned int sampleRate;
		unsigned int bufferSize = 512;
	};
}

#endif
