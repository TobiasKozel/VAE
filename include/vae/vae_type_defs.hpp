#ifndef _VAE_TYPE_DEFS
#define _VAE_TYPE_DEFS

namespace vae {
	using Sample = float;							///< Default sample types used where ever possible, changing this means the engine needs to be recompiled, short and int isn't working, probably needs a few changes in the pipeline
	using Position = float;							///< Used for coordinate systems
	using Size = unsigned int;						///< How the elements are addressed in the heapbuffer
	using Time = double;							///< Time sotred in seconds
	using Channel = unsigned char;					///< Used to index channels, 255 should be enough channels given most suround setups need less than 10

	using BankHandle 			= unsigned char;	///< Allows for 255 banks (unloaded or loaded)
	using EventHandle 			= unsigned short;	///< The handle used to address events within a bank
	using SourceHandle 			= unsigned short;	///< Addresses Sources within a bank
	using EmitterHandle 		= unsigned int;		///< Emitters are addressed globally within the engine and hash mapped to the actual object.
	using MixerHandle			= unsigned char;	///< Mixer are addressed per bank so 255 is enough usually.
	using ListenerHandle		= unsigned char;	///< Addressed globally. I don't think there's ever a need for more than 4 listeners.

	using GlobalEventHandle		= unsigned int;		///< Used to globally address events, holds space for BankHandle and EventHandle
	using GlobalMixerHandle		= unsigned short;	///< TODO Allow user to address a mixer with a composite handle (bank+mixer).
	using GlobalParameterHandle = unsigned int;		///< TODO Allow user to address Effect parameter with a composite handle (bank+mixer+effectslot+effectparam)

	using Pointer				= void*;			///< Pointer type
	using CString				= const char*;		///<
}

#endif // _VAE_PUBLIC_TYPES
