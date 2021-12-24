// #define TKLB_MEM_MONKEY_PATCH

#include "vae/vae.hpp" // harmless public vae header
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include "../src/core/vae_engine.hpp" // basically the whole engine and most of the deps
#include "./vae_def.hpp" // generated bank definition

using namespace vae;

void eventTriggered(const EventCallbackData* data) {
	// printf("callback event %ifrom bank %i\n", (int) data->event, (int) data->bank);
}


constexpr int test = sizeof(core::Engine);
// constexpr int rate = 48000;
constexpr int rate = 44100;
const double step = 1.0 / double(rate);

LocationDirection randomVec() {
	float x = (rand() / float(RAND_MAX) - 0.5) * 200;
	float y = (rand() / float(RAND_MAX) - 0.5) * 200;
	float z = (rand() / float(RAND_MAX) - 0.5) * 200;
	return {{ x, y, z }, {} };
}

int main() {
	EngineConfig config;
	#ifdef _MSC_VER
		config.rootPath = "../../../dev/";
	#else
		config.rootPath = "../../dev/";
	#endif
	config.eventCallback = &eventTriggered;
	config.internalSampleRate = rate;

	core::Engine engine;
	engine.init(config);

	// auto hrtf = engine.loadHRTF("hrtf.json");
	engine.start();
	auto result = engine.loadBank("bank1");
	const int emitterCount = 10000;
	EmitterHandle emitters[emitterCount];
	for (auto& i : emitters) {
		i = engine.createEmitter();
		float spread = rand() / float(RAND_MAX);

		engine.setEmitter(i, randomVec(), spread);
	}
	auto listener = engine.createListener();
	engine.setListener(listener, {});

	if (result == Result::Success) {
		for (int i = 0; i < 5000; i++) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000 / float(300)));
			float t = i * 0.1;
			engine.setEmitter(emitters[rand() % emitterCount], randomVec(), 0.5);
			engine.fireGlobalEvent(vaeb::Bank1::JumpRand, emitters[rand() % emitterCount]);
			if (i % 50 == 0) {
				auto em = emitters[rand() % emitterCount];
				engine.fireGlobalEvent(vaeb::Bank1::Ambient, em);
			}
			engine.update();
		}
	};
	engine.unloadBankFromId(0);

	engine.stop();

	return 0;
}
