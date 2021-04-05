#include "../../include/VAE/vae_listener.hpp"

#include "../core/vae_listener_impl.hpp"

namespace VAE {
	Listener::Listener(Impl::ListenerImpl* handle) {
		mHandle = handle;
	}

	Listener::~Listener() {

	}

	bool Listener::openDevice() {
		return mHandle->openDevice();
	}

	bool Listener::openDevice(const DeviceInfo& device) {
		return false;
	}

	unsigned int Listener::getDeviceCount() {
		return 1;
	}

	DeviceInfo Listener::getDevice(int deviceIndex) {
		return DeviceInfo();
	}

	void Listener::destroy() {

	}
}
