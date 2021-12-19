# VAE
Virtual Audio Engine is a fairly WIP 3D softwareaudio rendererc.

It aims to be a fairly small and data driven Engine with a focus on performance.
Its design is mostly data oriented and tries to avoid inhertance and preallocates
almost all memory upfront.

Most of it one single compilation unit so the compilercangoon it's way and inline
whatever it wants to.
This also means extending it is done directly in source and not via interfaces or other common mechanisms.

## TODOs
- Mixer effects
- Parameter controls
- State
- Automation
- Gapless looping
- Variable speed playback
- Filtered voices
- Faster HRTF
- vorbis decoder
- streaming
- signal generators
- some sort of baisc authoring tool
- Obstruction
- Geometry and material system
- Virtual voices
- Voice priority system
- Seperate Mixing hirarchies for each listener
- Seperate Audio device per listener
- Inject audio stream at runtime (e.g. for VoiP)

## Features
- Simple json bank format
- 3D SPCAP or HRTF panning
- Events drive the sound
- upfront .wav loading, no streaming
- Simple mixer hirachry
- Multiple Listeners, no individual output devices however

## Defines

### VAE_RELEASE
- Disables all log levels except error
- Strips the name from structs
- Disables assertions
### VAE_LOG_EVENTS
Logs all events

### VAE_DLL_EXPORT
Only relevant for the PIMPL Api, will add __declspec to all funcitons
