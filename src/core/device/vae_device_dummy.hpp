#ifndef VAEZ_DEVICE_DUMMY
#define VAEZ_DEVICE_DUMMY

#include "./vae_device.hpp"

namespace vae { namespace core {
	class BackendDummy : public Backend {
		BackendDummy() { }
	public:
		static BackendDummy& instance() {
			static BackendDummy backend;
			return backend;
		}

		uint getDeviceCount() override { return 1; }

		DeviceInfo getDevice(uint id) override {
			TKLB_ASSERT(id == 0) // there's only one device
			DeviceInfo info;
			info.channelsIn = 2;
			info.channelsOut = 2;
			tklb::memory::stringCopy(info.name, "Dummy Device", sizeof(DeviceInfo::name));
			tklb::memory::stringCopy(info.api, getName(), sizeof(DeviceInfo::api));
			return info;
		}

		const char* getName() override  { return "dummy"; };

		DeviceInfo getDefaultDevice() override {
			return getDevice(0);
		};
	};

	/**
	 * @brief Backend without functionality
	 */
	class DeviceDummy final : public Device {
	public:
		DeviceDummy() : Device(BackendDummy::instance()) { }

		DeviceDummy(SyncCallback& callback) : Device(callback, BackendDummy::instance()) { }

		bool openDevice(uint output = 2, uint input = 0) override {
			init(Config::SampleRate, input, output);
			return true;
		}

		bool openDevice(DeviceInfo& device) override {
			// there's only one device
			TKLB_ASSERT(device.id == 0)
			return openDevice(device.channelsOut, device.channelsIn);
		}

		bool closeDevice() override { return true; }
	};
} } // VAE::core

#endif // VAEZ_DEVICE_DUMMY
