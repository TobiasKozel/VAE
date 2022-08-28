/**
 * @file minimal.cpp
 * @author Tobias Kozel
 * @brief Minimal version of the engine with just about
 *        everything stripped
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#define VAE_IMPL
#define VAE_RELEASE
#define VAE_NO_STDLIB
#define VAE_NO_STDIO
#define VAE_NO_SIMD
#define VAE_NO_EFFECTS
#define VAE_NO_AUDIO_DEVICE
#define TKLB_RESAMPLER_LINEAR
#define VAE_NO_WAV
// #define VAE_NO_OGG
#define TKLB_NO_LOG
#define VAE_NO_HRTF
#define TKLB_ASSERT(...)

#include "../src/core/vae_engine.hpp" // basically the whole engine and most of the deps

// #include"../external/tklb/src/memory/TFixedPool.hpp"

// struct FixedPool {
// 	static constexpr size_t poolSize = 1024 * 1024 * 128; // 128 mb
// 	void* memory;
// 	tklb::memory::FixedPool pool;
// 	FixedPool() : memory(malloc(poolSize)), pool(memory, poolSize) { }
// 	~FixedPool() { free(memory); }
// } poolOwner;

// const char* BankDummy =
// 	"{\"id\": 0,\"name\": \"Bank1\",\"sources\": [{\"id\": 0,\"name\": \"Sine\",\"path\": \"sine.wav\",\"format\": \"wav\",\"resample\": true,\"gain\": 0.6}],"
// 	"\"mixers\": [{\"id\": 0, \"name\": \"Bank Master\"},"
// 	"\"events\": [{\"id\": 0,\"name\": \"JumpRand\",\"action\": \"start\",\"source\": 0}]}";

extern "C" void __cxa_pure_virtual() { while(1); }

void* tklb_malloc(size_t bytes) {
	// return malloc(bytes);
	return nullptr;
}

void tklb_free(void* ptr) {
	// free(ptr);
}

void* vae_file_open(const char* path, const char* mode) {
	return reinterpret_cast<void*>(3);
}

size_t vae_file_seek(void* file, size_t, int seek) {
	// return strlen(BankDummy); // size
	return 1;
}

size_t vae_file_read(char* dest, size_t size, int, void* file) {
	// memcpy(dest, BankDummy, size);
	return 1;
}

size_t vae_file_close(void* file) {
	return 0;
}

using namespace vae;

const int bufferSize = 512;
const int channels = 2;

int main() {
	int asd = 0;
	core::Engine engine;
	EngineConfig config;
	config.voices = 4;
	config.hrtfVoices = 0;
	config.preAllocatedEmitters = 4;
	config.virtualVoices = 0;
	config.finishedVoiceQueueSize = 4;

	engine.init(config);
	engine.start();
	engine.loadBank("bank");

	float buffer[bufferSize * channels];
	auto e = engine.createEmitter();
	for (int i = 0; i < 10000; i++) {
		engine.fireEvent(0, 0, e);
		engine.process(bufferSize, buffer, channels);
		engine.update();
	}
	return 0;
}
