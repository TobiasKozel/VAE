#include "../../include/vae/vae_pimpl.hpp"
#include <chrono>
#include <thread>
int main() {
	auto e = vae::EnginePimpl::create();
	e->start();
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(4000));
	e->stop(); // rtaudio seems to deadlock inside a dll
	e->destroy();
	return 0;
}
