#ifndef _VAE_BACKEND
#define _VAE_BACKEND

#include "../../../include/vae/vae.hpp"
#include "../vae_types.hpp"

namespace vae { namespace core {
	class Device;

	/**
	 * @brief Backend interface used to query devices before
	 * creating a actual device object
	 */
	class Backend {
	public:
		/**
		 * @brief Returns name of the api
		 */
		virtual const char* getName() const = 0;

		/**
		 * @brief Gets number of devices, needed to iterate them.
		 * Device index != does not have to be the device index!
		 */
		virtual Size getDeviceCount() = 0;

		/**
		 * @brief Returns a spefic device info for index.
		 */
		virtual DeviceInfo getDevice(Size index) = 0;

		virtual DeviceInfo getDefaultInputDevice() = 0;

		virtual DeviceInfo getDefaultOutputDevice() = 0;

		/**
		 * Creates a device instance for this backend
		 */
		virtual Device* createDevice(const EngineConfig&) = 0;
	}; // class Backend
} } // vae::core

#endif // _VAE_BACKEND
