#ifndef TKLBZ_DUMMY_BACKEND
#define TKLBZ_DUMMY_BACKEND

#include "./vae_backend.hpp"

namespace VAE { namespace core {
	/**
	 * @brief Backend without functionality
	 */
	class BackendDummy final : public Backend {
	public:
		BackendDummy() { }

		BackendDummy(SyncCallback& callback) : Backend(callback) { }

		bool openDevice() override { return true; }

		bool openDevice(const DeviceInfo& device) override {
			// there's only one device
			TKLB_ASSERT(device.id == 0)
			return openDevice();
		}

		uint getDeviceCount() override { return 1; }

		DeviceInfo getDevice(uint id) override {
			TKLB_ASSERT(id == 0) // there's only one device
			DeviceInfo dummy;
			dummy.channels = 2;
			// dummy.name = "Dummy Device";
			return dummy;
		}

		bool closeDevice() override { return true; }
	};
} } // VAE::core

#endif // TKLBZ_DUMMY_BACKEND
