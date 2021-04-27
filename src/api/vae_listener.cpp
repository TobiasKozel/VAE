#include "../../include/VAE/vae_listener.hpp"

#include "../core/vae_listener_impl.hpp"
#include "../../external/tklb/src/memory/TMemory.hpp"

namespace VAE {
	Listener::Listener(Impl::ListenerImpl* pimpl) {
		mPimpl = pimpl;
	}

	bool Listener::openDevice() {
		return mPimpl->openDevice();
	}

	bool Listener::openDevice(const DeviceInfo& device) {
		return mPimpl->openDevice(device);
	}

	unsigned int Listener::getDeviceCount() {
		return mPimpl->getDeviceCount();
	}

	DeviceInfo Listener::getDevice(int deviceIndex) {
		return DeviceInfo();
	}

	void Listener::destroy() {
		TKLB_DELETE(mPimpl);
	}
}
