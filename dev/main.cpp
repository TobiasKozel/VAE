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
	float x = (rand() / float(RAND_MAX) - 0.5) * 10;
	float y = (rand() / float(RAND_MAX) - 0.5) * 10;
	float z = (rand() / float(RAND_MAX) - 0.5) * 10;
	return {{ x, y, z }, {} };
}

void sleep(double ms) {
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(ms));
}

void benchmark(vae::core::Engine& engine) {
	const int emitterCount = 10000;
	EmitterHandle emitters[emitterCount];
	for (auto& i : emitters) {
		i = engine.createEmitter();
		float spread = rand() / float(RAND_MAX);

		engine.setEmitter(i, randomVec(), spread);
	}
	for (int i = 0; i < 2000; i++) {
		sleep(2);
		float t = i * 0.1;
		engine.setEmitter(emitters[rand() % emitterCount], randomVec(), 0.5);
		engine.fireGlobalEvent(vaeb::Bank1::JumpRand, emitters[rand() % emitterCount]);
		if (i % 3 == 0) {
			auto em = emitters[rand() % emitterCount];
			engine.fireGlobalEvent(vaeb::Bank1::Ambience1, em);
		}
		engine.update();
	}
}

void filterTest(vae::core::Engine& engine) {
	auto emitter = engine.createEmitter();
	auto emitter2 = engine.createEmitter();
	engine.fireGlobalEvent(vaeb::Bank1::Ambience1, emitter2);
	// engine.fireGlobalEvent(vaeb::Bank1::ShortSineLoop, emitter, 0.3);
	for (int i = 0; i < 2000; i++) {
		sleep(30);
		// engine.setHighpass(emitter, sin(i * 0.1) * 0.5 + 0.5);
		engine.setSpeed(emitter, sin(i * 0.3) * 0.2 + 1.0);
		if (i % 100 == 0) {
			// engine.fireGlobalEvent(vaeb::Bank1::JumpRand, emitter);
		}
		engine.setMixerEffectParameter(0, 3, 0, 3, sin(i) * 0.45 + 0.5);
	}
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

	auto listener = engine.createListener();
	engine.setListener(listener, {});
	// auto hrtf = engine.loadHRTF("hrtf.json");
	engine.start();
	auto result = engine.loadBank("bank1");

	if (result == Result::Success) {
		// benchmark(engine);
		filterTest(engine);
	};
	engine.unloadBankFromId(0);

	engine.stop();

	return 0;
}
