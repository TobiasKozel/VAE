#include "../../include/VAE/vae_engine.hpp"

#include "../../external/tklb/src/memory/TMemory.hpp"
#include "../core/vae_logger.hpp"
#include "../core/vae_engine_impl.hpp"
#include "../core/vae_listener_impl.hpp"

namespace VAE {
	Engine::Engine() {

	}

	Engine::Engine(void* fixedMemory, size_t blockSize) {
		TKLB_ASSERT(false) // TODO tklb not implemented
	}

	Engine::Engine(
		void* (*allocate)  (size_t),
		void* (*reallocate)(void*, size_t),
		void  (*deallocate)(void*)
	) {
		TKLB_ASSERT(false) // TODO tklb not implemented
	}

	void Engine::setLogCallback(void (*log)(const char* msg, int channel, int level)) {
		Impl::Logger::setCallback(log);
	}

	void Engine::destroy() {

	}

	Listener Engine::createListener() {
		return Listener(TKLB_NEW(Impl::ListenerImpl));
	}

	Emitter Engine::createEmitter() {
		return Emitter(TKLB_NEW(Impl::EmitterImpl));
	}
}
