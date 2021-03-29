#ifndef VAE_API_ENGINE
#define VAE_API_ENGINE

#include <cstddef>
#include "./device.hpp"

namespace VAE {
	class Engine {
		void* mEngine = nullptr;

	public:
		// TODO These Contructors might need compiler switches since
		// TODO function pointers for allocation routines seem slow

		/**
		 * @brief Using stdlib for allocations
		 */
		Engine();

		/**
		 * @breif Using a preallocated fixed block of memory.
		 * Once memory runs out sounds might not play.
		 */
		Engine(void* fixedMemory, size_t blockSize);

		/**
		 * @brief Memory allocation using provided routines.
		 */
		Engine(
			void* (*_malloc)(size_t),
			void* (*_realloc)(void*, size_t),
			void (*_free)(void*)
		);


		~Engine();

		/**
		 * @brief Will open the default audio device.
		 * Don't call process() after calling this.
		 */
		void openDevice();

		/**
		 * @brief Open a specific output device.
		 * Can also be used to change the current device.
		 */
		void openDevice(const Device& device);

		/**
		 * @brief Total device count used to iterate over them.
		 */
		unsigned int getDeviceCount();

		/**
		 * @brief Get a deivce by index.
		 */
		Device getDevice(unsigned int deviceIndex);



		/**
		 * @brief Custom callback, in case there are audio device is already present
		 * or offline rendering.
		 * Don't call openDevice() when using this.
		 */
		void process(const float** in, float** out, unsigned int frames);


	};
}

#endif
