# VAE Readme
Virtual Audio Engine is a fairly WIP 3D software-audio-renderer.

Minimal example to play some sound
```C++
#include <thread>
#include "vae/vae_pimpl.hpp"	// Pimpl API
#include "./vae_def.hpp"		// generated bank names from json (generate_bank_defines.py)

int main() {
	vae::EnginePimpl e = vae::EnginePimpl::create();
	e->init();	// Uses default settings and default audio device
	e->start();	// Starts audio
	auto result = e->loadBank("bank1"); // open ./bank1/bank.json
	result = e->loadBank("bank2");		// open ./bank2/bank.json
	// see /dev/bank1/ and /dev/bank2/ for examples

	if (result != Result::Success) { return 1; }
	auto emitter = e->createEmitter();
	auto listener = e->createListener(); // only needed for spatialized sounds
	e->fireGlobalEvent(vaeb::Bank1::Music, emitter); // fire named event
	e->setSpeed(emitter, 0.6); // slow the music

	// let it run for a little
	for (int i = 0; i < 2000, i++) {
		if (i % 100 == 0) {
			// Fire event 0 on bank 0, will not be slowed down
			e->fireEvent(0, 0, emitter);
		}
		e->update(); // update engine state
		// sleep a little
		std::this_thread::sleep_for(
			std::chrono::duration<double, std::milli>(1.0 / 60.0)
		);
	}
	e->destroy(); // clean up
	return 0;
}

```

It aims to be a fairly small and data driven Engine with a focus on performance.
Its design is mostly data oriented and tries to avoid inhertance and preallocates
almost all memory upfront and while loading banks.

Most of it inside one single compilation unit, so the compiler can go on it's way and inline
whatever it wants to.
This also means extending it is done directly in source and not via interfaces or other common mechanisms (besides AudioDevice and EffectBase which are interfaces).

The easiest way to get started is to compile a shared library with
```
mkdir build
cd build
cmake -DVAE_BUILD_SHARED_LIBS=ON ..
make
```
And then use the vae::EnginePimpl class in include/vae/vae_pimpl.hpp while linking against the shared libvae.so/dll library.


Alternatively you can add the whole thing as a subproject and static link against it.
Using the pimpl api is optional but the better option since it won't pull in the whole thing.

[doxygen page](https://tobiaskozel.github.io/VAE-Docs/)

## TODOs
- Fix click at end of sounds for speex reampled sounds
- Split up pod data and internal data structures so they can be exposed to the public API as well.
- Parameter controls and smoothing
- progress virtual voices and kill them so they don't pile up
- Event queue for synchronized playback
- State
- Automation
- Faster HRTF (currently in time domain and about 80 times slower than a normal voice)
- HRTF interoplation
- streaming
- some sort of basic authoring tool
- Geometry and material system for obstruction and reverb zones

## Features
- 3D SPCAP or HRTF panning (slow implementation)
- Event driven sound system
- Simple json bank format
- upfront .wav and .ogg loading, no streaming
- Simple mixer hirachry
- Fixed mixrate with high quality resampler for deviceoutput (speex)
- High quality resampling when loading audio
- Realtime linear resampling otherwise
- Per voice variable speed playback and high/lowpass filters
- Multiple Listeners, no individual output devices however and not tested at all
- Integration for the profiler tracy which can give nice insights on memory consumtion and cpu times

## Performance
- Figure out why SPCAP voices are faster than Default voices, test mono and stereo.
- deduplicate audio rendering code

## Nice to haves
- signal generators
- Inject audio stream at runtime (e.g. for VoiP)
- Voice priority system (got unkillable voice at least)
- Seperate Mixing hirarchies for each listener
- Seperate Audio device per listener

## Defines

### VAE_RELEASE
- Disables all log levels except error
- Strips the name from structs
- Disables assertions
### VAE_LOG_EVENTS
Logs all events

### VAE_LOG_VOICES
Logs voices starting and stopping

### VAE_DLL_EXPORT
Only relevant for the PIMPL Api, will add __declspec to all funcitons

### VAE_NO_AUDIO_THREAD
Does remove ability to render audio in a seperate thread if vae::EngineConfig::processInBufferSwitch is false
the processing function has to be called manually (TODO)

### VAE_NO_AUDIO_DEVICE
Only makessense with VAE_NO_AUDIO_THREAD to drive the engine from the outside.
Will expose vae::core::Engine::process which can be called from a custom callback.

Not pretty and goes against the device abstraction idea

### VAE_NO_EXCEPT
Tries to disable exceptions in third party libs, should build with -fno-exceptions

### VAE_NO_SIMD
Disables SIMD

### VAE_NO_STDIO
Tries to get rid of most stdio, hash map currently still depends on it

File reading and writing need to be provided by defining the functions vae_fs.hpp

Also requires VAE_PRINT to be defined

### VAE_PRINT

Can be defined to redirect logging, same interface as printf

