#include "vae/vae_type_defs.hpp"
#define VAE_FORCE_LOG
#define VAE_IMPL

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
constexpr int rate = 48000;
// constexpr int rate = 44100;
const double step = 1.0 / double(rate);

LocationDirection randomVec() {
	float x = (rand() / float(RAND_MAX) - 0.5) * 10;
	float y = (rand() / float(RAND_MAX) - 0.5) * 10;
	float z = (rand() / float(RAND_MAX) - 0.5) * 10;
	return {{ x, y, z }, {} };
}

void sleepMs(double ms) {
	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(ms));
}


// Play one unfiltered voice
void benchmarkBasicVoice(vae::core::Engine& engine) {
	auto emitter = engine.createEmitter();
	for (int i = 0; i < 2000; i++) {
		sleepMs(1);
		engine.fireGlobalEvent(vaeb::Bank1::JumpRand, emitter);
		engine.update();
	}
}

// play a bunch of long, short and spatialized sounds
void benchmark(vae::core::Engine& engine) {
	const int emitterCount = 10000;
	EmitterHandle emitters[emitterCount];
	for (auto& i : emitters) {
		i = engine.createEmitter();
		float spread = rand() / float(RAND_MAX);

		engine.setEmitter(i, randomVec(), spread);
	}
	for (int i = 0; i < 2000; i++) {
		sleepMs(2);
		float t = i * 0.1;
		engine.setEmitter(emitters[rand() % emitterCount], randomVec(), 0.5);
		engine.fireGlobalEvent(vaeb::Bank1::JumpRand, emitters[rand() % emitterCount]);
		if (i % 3 == 0) {
			auto em = emitters[rand() % emitterCount];
			engine.fireGlobalEvent(vaeb::Bank2::Ambience1, em);
		}
		engine.update();
	}
}

void filterTest(vae::core::Engine& engine) {
	auto emitter = engine.createEmitter();
	auto emitter2 = engine.createEmitter();
	engine.fireGlobalEvent(vaeb::Bank2::Music, emitter2);
	engine.fireGlobalEvent(vaeb::Bank1::ShortSineLoop, emitter, 0.3);
	for (int i = 0; i < 2000; i++) {
		sleepMs(30);
		engine.setHighpass(emitter, sin(i * 0.1) * 0.5 + 0.5);
		engine.setSpeed(emitter, sin(i * 0.3) * 0.2 + 1.0);
		if (i % 100 == 0) {
			engine.fireGlobalEvent(vaeb::Bank1::JumpRand, emitter);
		}
		engine.setMixerEffectParameter(0, 3, 0, 3, sin(i) * 0.45 + 0.5);
	}
}

void test3d(vae::core::Engine& engine, vae::ListenerHandle listener) {
	auto emitter = engine.createEmitter();
	engine.fireEvent(1, 6, emitter);

	const float spread = 0.2;
	const float distance = 0.2;

	TKLB_INFO("front")
	engine.setEmitter(emitter, {{ 0, distance, 0 }, {}}, spread);
	sleepMs(2000);

	TKLB_INFO("Left")
	// engine.setListener(listener, {{ 1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }});
	engine.setEmitter(emitter, {{ -distance, 0, 0}, {}}, spread);
	sleepMs(2000);

	TKLB_INFO("right")
	// engine.setListener(listener, {{ -1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }});
	engine.setEmitter(emitter, {{ +distance, 0, 0}, {}}, spread);
	sleepMs(2000);

	TKLB_INFO("front right")
	engine.setEmitter(emitter, {{ +distance, distance, 0}, {}}, spread);
	sleepMs(2000);

	TKLB_INFO("front left")
	engine.setEmitter(emitter, {{ -distance, distance, 0}, {}}, spread);
	sleepMs(2000);

	engine.stopEmitter(emitter);
}

int main() {
	EngineConfig config;
	#ifdef _MSC_VER
		config.rootPath = "../../../dev/";
	#endif
	config.eventCallback = &eventTriggered;
	config.internalSampleRate = rate;
	config.processInBufferSwitch = true;

	/**
	 * @brief very low settings
	 */
	// config.hrtfVoices = 0;
	// config.voices = 8;
	// config.preAllocatedEmitters = 128;
	// config.virtualVoices = 32;
	// config.bufferPeriods = 2;
	// config.finishedVoiceQueueSize = 16;
	// config.preferredBufferSize = 128;

	core::Engine engine;
	engine.init(config);
	engine.setMasterVolume(4);

	auto listener = engine.createListener();
	engine.setListenerConfiguration(listener, SpeakerConfiguration::Headphones);
	// engine.setListener(listener, {
	// 	{ 0, 0,  0 },
	// 	{ 0, 0, +1 },
	// 	{ 0, +1,  0 }
	// });

	// auto hrtf = engine.loadHRTF("hrtf.json");

	auto result = engine.loadBank("bank1");
	if (result != Result::Success) { return 1; }
	result = engine.loadBank("bank2");
	if (result != Result::Success) { return 1; }

	engine.start();

	// benchmarkBasicVoice(engine);
	// benchmark(engine);
	// filterTest(engine);
	test3d(engine, listener);

	engine.unloadBank("bank1");

	engine.stop();

	return 0;
}
