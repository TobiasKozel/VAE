#include <stdio.h>
#include <thread>
#include "../src/core/vae_engine.hpp"

using namespace vae;

void eventTriggered(EventCallbackData data) {
	printf("callback event %i %s from bank %i\n", (int) data.event, data.name, (int) data.bank);
}

int main() {
	constexpr int test = sizeof(core::Engine);
	constexpr int rate = 44100;
	tklb::AudioBuffer buffer;
	buffer.resize(rate, 1);
	buffer.sampleRate = rate;
	for (int i = 0; i < buffer.size(); i++) {
		buffer[0][i] = sin(i * 0.1) * 0.7;
	}
	tklb::wave::write(buffer, "../../../dev/bank1/sound1.wav");

	EngineConfig config;
	config.eventCallback = &eventTriggered;
	config.preferredSampleRate = rate;

	core::Engine engine(config);

	engine.init();

	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));

	if (engine.loadBank("../../../dev/bank1") == Result::Success) {
		printf("Waiting 2 secs\n");
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));

		engine.fireEvent(0, 0);

		printf("Polling for a little...\n");
		for (int i = 0; i < 200; i++) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(30));
			engine.update();
		}
		printf("Shutting down...\n");
	};

	engine.unloadBankFromId(0);

	int i = 0;

	return 0;
}
