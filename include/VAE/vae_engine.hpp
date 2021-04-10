#ifndef VAE_API_ENGINE
#define VAE_API_ENGINE

#define VAE_MEM_CUSTOM

#include <cstddef>
#include "./vae_listener.hpp"


namespace VAE {
	namespace Impl {
		class EngineImpl;
	}

	class Engine {
		Impl::EngineImpl* mHandle = nullptr;

	public:

	#if !defined(VAE_MEM_STANDART_ALLOC) && !defined(VAE_MEM_FIXED)
		/**
		 * @brief Using stdlib for allocations
		 */
		Engine();
	#endif

	#ifdef VAE_MEM_FIXED
		/**
		 * @breif Using a preallocated fixed block of memory.
		 * Once memory runs out sounds might not play.
		 */
		Engine(void* fixedMemory, size_t blockSize);
	#endif

	#ifdef VAE_MEM_CUSTOM
		/**
		 * @brief Memory allocation using provided routines.
		 */
		Engine(
			void* (*allocate)(size_t),
			void* (*reallocate)(void*, size_t),
			void (*deallocate)(void*)
		);
	#endif
		/**
		 * @brief Provide a log callback if it shouldn't write to stdout
		 * @param msg The log message.
		 * @param level The log level. Critical = 0, Warning = 1, Info = 2, Debug = 3
		 */
		void setLogCallback(void (*log)(const char* msg, int level));

		Listener createListener();

		~Engine();

		void destroy();
	};
}

#endif
