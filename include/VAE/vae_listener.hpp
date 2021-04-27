#ifndef VEA_API_LISTENER
#define VEA_API_LISTENER

#include "./vae_device_info.hpp"

namespace VAE {
	namespace Impl {
		class ListenerImpl;
	}

	class Listener {
		// Actual object managed by the engine
		Impl::ListenerImpl* mHandle = nullptr;
	public:
		Listener() = delete;

		Listener(Impl::ListenerImpl* pimpl);

		void openDevice();

		void openDevice(DeviceInfo& device);

		void closeDevice();

		int getDeviceCount();

		DeviceInfo getDevice(int id);

		void destroy();

	};

}

#endif
