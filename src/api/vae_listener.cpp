#include "../../include/VAE/vae_listener.hpp"

#include "../core/vae_listener_impl.hpp"
#include "../external/tklb/src/util/TMemory.hpp"

namespace VAE {
	Listener::Listener(Impl::ListenerImpl* handle) {
		mHandle = handle;
	}

	bool Listener::openDevice() {
		return mHandle->openDevice();
	}

	bool Listener::openDevice(const DeviceInfo& device) {
		return mHandle->openDevice(device);
	}

	unsigned int Listener::getDeviceCount() {
		return mHandle->getDeviceCount();
	}

	DeviceInfo Listener::getDevice(int deviceIndex) {
		return DeviceInfo();
	}

	void Listener::destroy() {
		TKLB_DELETE(mHandle);
	}
}
