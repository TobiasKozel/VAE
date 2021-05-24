#ifndef VAE_API_DEVICE_INFO
#define VAE_API_DEVICE_INFO

namespace vae {
	struct DeviceInfo {
		/**
		 * Negative values for invalid device.
		 */
		int id;
		unsigned int channelsIn;
		unsigned int channelsOut;
		unsigned int sampleRate;
		char name[255];
		char api[4]; // API abbreviation
		unsigned int bufferSize = 512;
	};
}

#endif
