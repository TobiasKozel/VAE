#ifndef _VAE_TYPE_DEFS
#define _VAE_TYPE_DEFS

namespace vae {
	using Sample = float;						///< Default sample types used where ever possible, changing this means the engine needs to be recompiled, short and int isn't working, probably needs a few changes in the pipeline
	using Size = unsigned int;					///< How the elements are addressed in the heapbuffer
	using Time = double;						///< Time sotred in seconds

	using SmallHandle		= unsigned char;
	using GenericHandle 	= unsigned short;
	using LargeHandle		= unsigned int;

	using BankHandle 			= SmallHandle;
	using EventHandle 			= GenericHandle;	///< The handle used to address events within a bank
	using SourceHandle 			= GenericHandle;
	using EmitterHandle 		= LargeHandle;
	using MixerHandle			= SmallHandle;
	using ListenerHandle		= SmallHandle;

	using GlobalEventHandle		= LargeHandle;		///< Used to globally address events, holds space for BankHandle and EventHandle
	using GlobalMixerHandle		= GenericHandle;	///< TODO
	using GlobalParameterHandle = LargeHandle;		///< TODO
}

#endif // _VAE_PUBLIC_TYPES
