#include <stdio.h>
#include "vae/vae_engine.hpp"
#include "vae/vae_listener.hpp"

int main() {
	auto engine = VAE::Engine();
	auto asd = engine.createListener();
	asd.openDevice();
	getchar();
	return 0;
}
