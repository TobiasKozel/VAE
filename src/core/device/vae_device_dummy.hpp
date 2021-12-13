#ifndef VAEZ_DEVICE_DUMMY
#define VAEZ_DEVICE_DUMMY

#include "./vae_device.hpp"

namespace vae { namespace core {
	/**
	 * @brief Backend without functionality
	 */
	class DeviceDummy final : public Device {
	public:
		DeviceDummy(
			Backend& backend, const EngineConfig& config
		) : Device(backend, config) { }

		bool openDevice(bool input = false) override {
			init(mConfig.internalSampleRate, 0, 2, Config::MaxBlock);
			return true;
		}

		bool openDevice(DeviceInfo& device) override {
			// there's only one device
			TKLB_ASSERT(device.id == 0)
			return openDevice(false);
		}

		bool closeDevice() override { return true; }

		/**
		 * @brief Simulate the callback from the audio device
		 *
		 * @tparam T
		 * @param from
		 * @param to
		 * @param frames
		 */
		template <typename T>
		void swapBufferInterleaved(const T* from, T* to, Size frames) {
			mWorker.swapBuffer<T>(from, to, frames);
		}
	};

	class BackendDummy : public Backend {
		BackendDummy() { }
	public:
		static BackendDummy& instance() {
			static BackendDummy backend;
			return backend;
		}

		uint getDeviceCount() override { return 1; }

		DeviceInfo getDevice(uint id) override{
			TKLB_ASSERT(id == 0) // there's only one device
			DeviceInfo info;
			info.channelsIn = 2;
			info.channelsOut = 2;
			tklb::memory::stringCopy(info.name, "Dummy Device", sizeof(DeviceInfo::name));
			tklb::memory::stringCopy(info.api, getName(), sizeof(DeviceInfo::api));
			return info;
		}

		const char* getName() const override  { return "dummy"; };

		DeviceInfo getDefaultInputDevice() override {
			return getDevice(0);
		};

		DeviceInfo getDefaultOutputDevice() override {
			return getDevice(0);
		};

		Device* createDevice(const EngineConfig& config) override {
			return new DeviceDummy(*this, config);
		}
	};
} } // VAE::core

#endif // VAEZ_DEVICE_DUMMY
