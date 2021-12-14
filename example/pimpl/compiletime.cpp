#include "../../include/vae/vae_pimpl.hpp"

int main() {
	auto e = vae::EnginePimpl::create();
	e->start();
	e->stop(); // rtaudio seems to deadlock inside a dll
	e->destroy();
	return 0;
}
