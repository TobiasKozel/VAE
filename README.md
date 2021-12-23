# VAE Readme
Virtual Audio Engine is a fairly WIP 3D softwareaudio rendererc.

It aims to be a fairly small and data driven Engine with a focus on performance.
Its design is mostly data oriented and tries to avoid inhertance and preallocates
almost all memory upfront.

Most of it one single compilation unit so the compilercangoon it's way and inline
whatever it wants to.
This also means extending it is done directly in source and not via interfaces or other common mechanisms.

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

## TODOs
- Fix click at end of sounds for speex reampled sounds
- Mixer effects
- Split up pod data and internal data structures so they can be exposed to the public API as well.
- Parameter controls and smoothing
- State
- Automation
- Faster HRTF and interpolation
- streaming
- some sort of basic authoring tool
- Geometry and material system for obstruction and reverb zones

## Nice to haves
- signal generators
- Inject audio stream at runtime (e.g. for VoiP)
- Voice priority system (got unkillable voice at least)
- Seperate Mixing hirarchies for each listener
- Seperate Audio device per listener


## Features
- 3D SPCAP or HRTF panning (slow implementation)
- Event driven sound system
- Simple json bank format
- upfront .wav and .ogg loading, no streaming
- Simple mixer hirachry
- Fixed mixrate with high quality resampler for deviceoutput (speex)
- High quality resampling when loading audio
- Realtime linear resampling otherwither
- Per voice variable speed playback and high/lowpass filters
- Multiple Listeners, no individual output devices however and not tested at all

## Defines

### VAE_RELEASE
- Disables all log levels except error
- Strips the name from structs
- Disables assertions
### VAE_LOG_EVENTS
Logs all events

### VAE_DLL_EXPORT
Only relevant for the PIMPL Api, will add __declspec to all funcitons
