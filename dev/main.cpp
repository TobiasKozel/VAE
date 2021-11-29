#include <stdio.h>
#include <thread>
#include "../src/core/vae_engine.hpp"

using namespace vae;

void eventTriggered(const EventCallbackData* data) {
	printf("callback event %ifrom bank %i\n", (int) data->event, (int) data->bank);
}

int main() {
	constexpr int test = sizeof(core::Engine);
	constexpr int rate = 48000;
	// constexpr int rate = 44100;
	const double step = 1.0 / double(rate);
	tklb::AudioBuffer buffer;
	buffer.resize(rate, 1);
	buffer.sampleRate = rate;
	for (unsigned int i = 0; i < buffer.size(); i++) {
		buffer[0][i] = sin(i * step * 440 * 3.141 * 2) * 0.7;
	}
	tklb::wave::write(buffer, "../../../dev/bank1/sound1.wav");

	EngineConfig config;
	// config.eventCallback = &eventTriggered;
	config.preferredSampleRate = rate;

	core::Engine engine(config);

	engine.init();

	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
	auto result = engine.loadBank("../../../dev/bank1");
	if (result == Result::Success) {
		printf("Waiting 2 secs\n");
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));

		// for (int i = 0; i < 1050; i++)
		{
			engine.fireEvent(0, 0);
		}

		printf("Polling for a little...\n");
		for (int i = 0; i < 200; i++) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(30));
			engine.update();
		}
	};

	engine.unloadBankFromId(0);

	int i = 0;

	return 0;
}
