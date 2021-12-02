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
	constexpr unsigned int rampLength = 10000;
	constexpr unsigned int rampStart = rampLength;
	const unsigned int rampEnd = buffer.size() - rampLength;
	for (unsigned int i = 0; i < buffer.size(); i++) {
		float gain = 0;
		if (i < rampStart) {
			gain = float(i) / float(rampLength);
		} else if (rampEnd < i) {
			gain = 1.f - (float(i - rampEnd) / float(rampLength));
		} else {
			gain = 1.0;
		}
		buffer[0][i] = sin(i * step * 440 * 3.141 * 2) * 0.7 * gain;
	}

	EngineConfig config;
	#ifdef _MSC_VER
		tklb::wave::write(buffer, "../../../dev/bank1/sound1.wav");
		config.rootPath = "../../../dev/";
	#else
		tklb::wave::write(buffer, "../../dev/bank1/sound1.wav");
		config.rootPath = "../../dev/";
	#endif

	// config.eventCallback = &eventTriggered;
	config.preferredSampleRate = rate;

	core::Engine engine(config);

	engine.start();

	auto result = engine.loadBank("bank1");

	auto emitter = engine.createEmitter();

	engine.setListener(0, {});

	if (result == Result::Success) {
		printf("Waiting 2 secs\n");
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));

		// for (int i = 0; i < 1050; i++)
		{
			engine.fireEvent(0, 0, emitter);
		}

		printf("Polling for a little...\n");
		for (int i = 0; i < 200; i++) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(30));
			engine.setEmitter(emitter, {{0, 0, float(i)}, {} }, 0);
			engine.update();
		}
	};

	engine.unloadBankFromId(0);

	int i = 0;

	return 0;
}
