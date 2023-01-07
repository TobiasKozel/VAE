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
#define VAE_NO_EFFECTS // TODO this should work
#define VAE_NO_AUDIO_DEVICE
#define TKLB_RESAMPLER_LINEAR
#define VAE_NO_WAV
#define VAE_NO_OGG
#define TKLB_NO_LOG
#define VAE_NO_HRTF
#define TKLB_ASSERT(...)

#include "../src/core/vae_engine.hpp" // basically the whole engine and most of the deps

using namespace vae;
using String = vae::core::PathString;

// #include"../external/tklb/src/memory/TFixedPool.hpp"

// struct FixedPool {
// 	static constexpr size_t poolSize = 1024 * 1024 * 128; // 128 mb
// 	void* memory;
// 	tklb::memory::FixedPool pool;
// 	FixedPool() : memory(malloc(poolSize)), pool(memory, poolSize) { }
// 	~FixedPool() { free(memory); }
// } poolOwner;

const char* BankDummy =
	"{\"id\": 0,\"name\": \"Bank1\",\"sources\": [{\"id\": 0,\"name\": \"Sine\",\"path\": \"sine.wav\",\"format\": \"wav\",\"resample\": true,\"gain\": 0.6}],"
	"\"mixers\": [{\"id\": 0, \"name\": \"Bank Master\"},"
	"\"events\": [{\"id\": 0,\"name\": \"JumpRand\",\"action\": \"start\",\"source\": 0}]}";

// TODO only needed for the faust code, but that one can't be used without std for now
// extern "C" void __cxa_pure_virtual() { while(1); }

#ifdef VAE_NO_STDLIB
	void* tklb_malloc(core::SizeT bytes) {
		return new char[bytes];
	}

	void tklb_free(void* ptr) {
		delete[] (char*) ptr;
	}
#endif // VAE_NO_STDLIB

#ifdef VAE_NO_STDIO
	void* vae_file_open(const char* path, const char* mode) {
		String test(path, true);
		if (test.contains("sine.wav")) {
			return reinterpret_cast<void*>(3);
		}
		return reinterpret_cast<void*>(0);
	}

	core::SizeT vae_file_seek(void* file, core::SizeT, int seek) {
		if (reinterpret_cast<unsigned long>(file) == 3) {
			auto dummy = String(BankDummy, true);
			return dummy.size();
		}
		return 1;
	}

	core::SizeT vae_file_read(char* dest, core::SizeT size, int, void* file) {
		if (reinterpret_cast<unsigned long>(file) == 3) {
			tklb::memory::copy(dest, BankDummy, String(BankDummy, true).size());
		}
		return 0;
	}

	core::SizeT vae_file_close(void* file) {
		return 0;
	}
#endif // VAE_NO_STDIO

const int bufferSize = 512;
const int channels = 2;

int main() {
	core::Engine engine;
	EngineConfig config;
	config.voices = 4;
	config.hrtfVoices = 0;
	config.preAllocatedEmitters = 4;
	config.virtualVoices = 0;
	config.finishedVoiceQueueSize = 4;

	engine.init(config);
	engine.start();
	// engine.loadBank("bank");

	float buffer[bufferSize * channels];
	auto e = engine.createEmitter();
	for (int i = 0; i < 10000; i++) {
		engine.fireEvent(0, 0, e);
		engine.process(bufferSize, channels, buffer);
		engine.update();
	}
	return 0;
}
