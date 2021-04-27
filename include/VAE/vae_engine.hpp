#ifndef VAE_API_ENGINE
#define VAE_API_ENGINE

#define VAE_MEM_CUSTOM

#include <cstddef>
#include "./vae_listener.hpp"
#include "./vae_emitter.hpp"


namespace VAE {
	namespace Impl {
		class EngineImpl;
	}

	class Engine {
		Impl::EngineImpl* mPimpl = nullptr;

	public:

		/**
		 * @brief Using stdlib for allocations
		 */
		Engine();

		/**
		 * @brief Using a preallocated fixed block of memory.
		 * Once memory runs out sounds might not play.
		 */
		Engine(void* fixedMemory, size_t blockSize);
		/**
		 * @brief Memory allocation using provided routines.
		 */
		Engine(
			void* (*allocate)(size_t),
			void* (*reallocate)(void*, size_t),
			void (*deallocate)(void*)
		);
		/**
		 * @brief Provide a log callback if it shouldn't write to stdout
		 * @param msg The log message.
		 * @param channel Log channel
		 * @param level The log level. Critical = 0, Warning = 1, Info = 2, Debug = 3
		 */
		void setLogCallback(void (*log)(const char* msg, int channel, int level));

		Listener createListener();

		Emitter createEmitter();

		void destroy();
	};
}

#endif
