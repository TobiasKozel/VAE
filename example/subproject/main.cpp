#include <stdio.h>
#include "vae/vae_pimpl.hpp"

int main() {
	auto engine = vae::EnginePimpl::create();
	engine->start();
	getchar();
	engine->stop();
	return 0;
}
