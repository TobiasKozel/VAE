#define TKLB_MEM_MONKEY_PATCH
#include "../external/tklb/src/memory/TMemory.hpp"

#include <stdio.h>
#include <thread>
#include "../src/core/vae_engine.hpp"

using namespace vae;

void eventTriggered(const EventCallbackData* data) {
	printf("callback event %ifrom bank %i\n", (int) data->event, (int) data->bank);
}

void logMem() {
	static int point = 0;
	using lu = unsigned long;
	printf("Memory at %i \t %lu bytes\n",
		point,
		lu(tklb::memory::DefaultPoolDebug.getAllocated())
	);
	point++;
}

constexpr int test = sizeof(core::Engine);
// constexpr int rate = 48000;
constexpr int rate = 44100;
const double step = 1.0 / double(rate);

void generateWav() {
	constexpr auto length = core::Config::MaxBlock * 128;
	tklb::AudioBuffer buffer;
	buffer.resize(length , 1);
	buffer.sampleRate = rate;
	constexpr unsigned int rampLength = 100;
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
	#ifdef _MSC_VER
		tklb::wave::write(buffer, "../../../dev/bank1/sound1.wav");
	#else
		tklb::wave::write(buffer, "../../dev/bank1/sound1.wav");
	#endif
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

	core::Engine engine(config); logMem();

	// auto hrtf = engine.loadHRTF("hrtf.json"); logMem();

	engine.start(); logMem();

	auto result = engine.loadBank("bank1"); logMem();

	auto emitter = engine.createAutoEmitter(0, 0, 40, {{0.f, 0.f, 100.f}, {}}, 0.5); logMem();

	auto listener = engine.createListener(); logMem();

	engine.setListener(listener, {}); logMem();

	if (result == Result::Success) {
		// for (int i = 0; i < 1050; i++)
		{
			engine.fireEvent(0, 0, emitter);
		}

		for (int i = 0; i < 2000; i++) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000 / float(60)));
			float t = i * 0.1;
			engine.setEmitter(emitter, {{ float(sin(t)), float(0), float(cos(t)) }, {} }, 0);
			// engine.update(); // needs to be ticked if EngineConfig::updateInAudioThread is false in
		}
	};

	engine.unloadBankFromId(0);
	logMem();

	int i = 0;

	return 0;
}
