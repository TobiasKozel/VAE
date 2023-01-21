/**
 * @file vae_type_defs.hpp
 * @author Tobias Kozel
 * @brief Types and Handles used in the engine are defined here.
 * @details In order to preserve memory all types are as narrow as possible,
 *          which comes with some restrictions.
 *          However it should be possible to simply use a wider type here
 *          and recompile the entire sound engine to rise the limit.
 * @version 0.0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _VAE_TYPE_DEFS
#define _VAE_TYPE_DEFS

namespace vae {
	using Sample = float;							///< Default internal sample types used where ever possible to avoid conversions, non floating point types may compile but won't work probably.
	using Size = unsigned int;						///< How the elements are addressed in the heapbuffer
	using SampleIndex = unsigned int;				///< Maximal amount of addressable samples in a single audio file, 32 Bit means around 23 Hours of audio at 48kHz. If more is needed, this has to be changed. (Untested)
	using Time = double;							///< Time in seconds
	using Position = float;							///< Coordinates use this type. If the game engine uses double, this might be useful to change.

	using BankHandle 			= unsigned char;	///< Allows for 255 banks to be addressed
	using EventHandle 			= unsigned short;	///< The handle used to address events within a bank, (ca 65k events per bank)
	using SourceHandle 			= unsigned short;	///< Addresses Sources within a bank
	using EmitterHandle 		= unsigned int;		///< Emitters are addressed globally within the engine. 8 bits are reserved for access validation so only 16 Million emitters can be addressed at runtime.
	using MixerHandle			= unsigned char;	///< Mixer are addressed per bank so 255 is enough usually.
	using ListenerHandle		= unsigned char;	///< Addressed globally. I don't think there's ever a need for more than 4 listeners.

	using GlobalEventHandle		= unsigned int;		///< Used to globally address events, holds space for BankHandle and EventHandle
	using GlobalMixerHandle		= unsigned short;	///< TODO Allow user to address a mixer with a composite handle (bank+mixer).
	using GlobalParameterHandle = unsigned int;		///< TODO Allow user to address Effect parameter with a composite handle (bank+mixer+effectslot+effectparam)

	using Pointer				= void*;			///< Pointer type defined for api wrappers
	using CString				= const char*;		///< C string defined for api wrappers
}

#endif // _VAE_PUBLIC_TYPES
