#ifndef VAE_API_DEVICE
#define VAE_API_DEVICE

#include "./vae_device_info.hpp"

namespace vae {
	class PublicDevice {
		PublicDevice() { }
		PublicDevice(PublicDevice&) { }
	public:
		bool valid();
		bool openDevice(int output = 2, int input = 0);
		bool openDevice(const DeviceInfo& device);
		bool closeDevice();
		int getChannelsOut();
		int getChannelsIn();
		int getStreamTime();
	};
} // namespace vae


#endif // VAE_API_DEVICE
