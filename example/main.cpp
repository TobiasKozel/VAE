#include <stdio.h>
#include "VAE/vae_engine.hpp"
#include "VAE/vae_listener.hpp"

int main() {
	auto engine = VAE::Engine();
	auto asd = engine.createListener();
	asd.openDevice();
	getchar();
	return 0;
}
