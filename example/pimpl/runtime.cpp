// #define VAE_DLL_EXPORT

#include "../../include/vae/vae_pimpl.hpp"
#include <dlfcn.h>

int main() {
	void* handle = dlopen("../../build/libvae.so", RTLD_LAZY);
	auto create = (vae::EnginePimpl* (*)()) dlsym(handle, "create");
	auto e = create();
	e->start(); // TODO this apparently needs a interface class in between
	e->stop();
	e->destroy();
	return 0;
}
