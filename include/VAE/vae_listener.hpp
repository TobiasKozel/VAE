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

		Listener(Impl::ListenerImpl* handle);

		/**
		 * @brief Will open the default audio device.
		 * Don't call process() when using an audio device.
		 */
		bool openDevice();

		/**
		 * @brief Open a specific output device.
		 * Can also be used to change the current device.
		 */
		bool openDevice(const DeviceInfo& device);

		/**
		 * @brief Total device count used to iterate over them.
		 */
		unsigned int getDeviceCount();

		/**
		 * @brief Get a deivce by index.
		 */
		DeviceInfo getDevice(int deviceIndex);

		/**
		 * @brief Custom callback, in case there are audio device is already present
		 * or offline rendering.
		 * Don't call openDevice() when using this.
		 * @param out Output buffer, can be null if
		 */
		void process(unsigned int frames, float** out = nullptr);

		/**
		 * @brief NOT IMPLEMENTED!
		 * Maybe in a distant future.
		 */
		void setSubtitleCallback(void (*subtitle)(const char* msg)) { }

		void destroy();

	};

}

#endif
