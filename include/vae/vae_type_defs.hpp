#ifndef _VAE_TYPE_DEFS
#define _VAE_TYPE_DEFS

namespace vae {
	using Sample = float;							///< Default internal sample types used where ever possible
	using Size = unsigned int;						///< How the elements are addressed in the heapbuffer
	using Time = double;							///< Time in seconds
	using Position = float;							///< Coordinates use this type. If the game engine uses double, this might be useful to change.

	using BankHandle 			= unsigned char;	///< Allows for 255 banks (unloaded or loaded)
	using EventHandle 			= unsigned short;	///< The handle used to address events within a bank
	using SourceHandle 			= unsigned short;	///< Addresses Sources within a bank
	using EmitterHandle 		= unsigned int;		///< Emitters are addressed globally within the engine. 8 bits are reserved for access validation so only 16 Million emitters can be addressed by default.
	using MixerHandle			= unsigned char;	///< Mixer are addressed per bank so 255 is enough usually.
	using ListenerHandle		= unsigned char;	///< Addressed globally. I don't think there's ever a need for more than 4 listeners.

	using GlobalEventHandle		= unsigned int;		///< Used to globally address events, holds space for BankHandle and EventHandle
	using GlobalMixerHandle		= unsigned short;	///< TODO Allow user to address a mixer with a composite handle (bank+mixer).
	using GlobalParameterHandle = unsigned int;		///< TODO Allow user to address Effect parameter with a composite handle (bank+mixer+effectslot+effectparam)

	using Pointer				= void*;			///< Pointer type defined for api wrappers
	using CString				= const char*;		///< C string defined for api wrappers
}

#endif // _VAE_PUBLIC_TYPES
