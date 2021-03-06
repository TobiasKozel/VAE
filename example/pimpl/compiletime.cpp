#include "../../include/vae/vae_pimpl.hpp"
#include <chrono>
#include <thread>

int main() {
	auto e = vae::EnginePimpl::create();
	e->start();
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(4000));
	e->stop(); // stopping is optional
	e->destroy();
	return 0;
}
