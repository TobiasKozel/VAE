# VAE /src/core/pod/
Plain Old Data structures which are used drive the sound engine.
When interacting with the engine directly, they can be created on the fly.
Otherwise they will all be included in bank files, which for now are stored in jsons.

To see how the work, it's best to look at the definition of the structs for now.

See vae::EnginePimpl::loadBank

## Bank
A collection of events, sources and mixers which can be loaded and unloaded programmatically.

See vae::core::Bank
## Source
A Audio source like a wav file.
Will be loaded as soon as the bank is loaded.

See vae::core::Source

## Event
A event which triggers source to be played in a certain way.

Can also trigger other events.

See vae::core::Event

## Mixer
Mixer channel wich combines signals, and at some point might allow effects processing.

See vae::core::Mixer

