#include "../../include/VAE/vae_engine.hpp"

#ifdef VAE_MEM_FIXED
	#include "../external/tklb/util/TMemoryManager.hpp"
#endif

#include "../external/tklb/util/TMemory.hpp"
#include "../core/vae_engine_impl.hpp"

namespace VAE {
#if !defined(VAE_MEM_STANDART_ALLOC) && !defined(VAE_MEM_FIXED)
	Engine::Engine() {
	}
#endif

#ifdef VAE_MEM_FIXED
	Engine::Engine(void* fixedMemory, size_t blockSize) {
		tklb::memory::manager::CustomSize = blockSize;
		tklb::memory::manager::CustomMemory =
			reinterpret_cast<unsigned char*>(fixedMemory);
		tklb::memory::manager::use();
	}
#endif

#ifdef VAE_MEM_CUSTOM
	Engine::Engine(
		void* (*allocate)  (size_t),
		void* (*reallocate)(void*, size_t),
		void  (*deallocate)(void*)
	) {
		tklb::memory::allocate = allocate;
		tklb::memory::reallocate == reallocate;
		tklb::memory::deallocate == deallocate;
	}
#endif

	void Engine::destroy() {

	}

	template <class T>
	T Engine::create() {
		return T();
	}
}
