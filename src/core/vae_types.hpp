/**
 * @file vae_types.hpp
 * @author Tobias Kozel
 * @brief Internal types used across VAE
 * @version 0.1
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _VAE_TYPES
#define _VAE_TYPES

#include "../../include/vae/vae.hpp"
#include "./vae_config.hpp"
#include "../wrapped/vae_tklb.hpp"
#include "./vae_allocators.hpp"


/**
 * @brief To reduce the size of structs, bool flags and enums are reduced to a single bit.
 *        Depending on the bittesting capabilities of a cpu this might have a performance
 *        impact not worth the decrease in memory usage.
 */
#ifndef VAE_NO_SMALL_STRUCTS
	#define VAE_SMALL(bits) :bits
#else
	#define VAE_SMALL(bits)
#endif // !VAE_NO_SMALL_STRUCTS

namespace vae { namespace core {
	using Uchar = unsigned char;
	using Uint = unsigned int;
	using Real = float;				///< Used for lerps and other things which need floating point

	/**
	 * @brief Buffer type to hold all the audio data meant for playback
	 */
	using AudioBuffer = tklb::AudioBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, memory::DEFAULT_ALIGNMENT, memory::AllocatorAudio<>,  SampleIndex
		>
	>;

	/**
	 * @brief Buffer type to do all the work like mixing, filtering and so on
	 */
	using ScratchBuffer = tklb::AudioBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, memory::DEFAULT_ALIGNMENT, memory::AllocatorScratch<>, SampleIndex
		>
	>;

	using RingBuffer = tklb::AudioRingBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, memory::DEFAULT_ALIGNMENT, memory::AllocatorScratch<>, SampleIndex
		>
	>;



	#ifdef VAE_RELEASE
		using NameString = tklb::StackString<0>;			///< Used for debug names
	#else
		using NameString = tklb::StackString<16>;			///< Used for debug names
	#endif // VAE_RELEASE

	/**
	 * @brief Default heap class used for all non audio signal alocations.
	 *        Limited to 32 bit addressable items, which should sofice.
	 */
	template <class T>
	using HeapBuffer = memory::HeapBuffer<
		T, memory::DEFAULT_ALIGNMENT, memory::AllocatorMain<>, Size
	>;

	/**
	 * @brief Default heap class used for all non audio signal alocations.
	 *        Limited to 32 bit addressable items, which should sofice.
	 */
	template <class T>
	using VoiceHeapBuffer = memory::HeapBuffer<
		T, memory::DEFAULT_ALIGNMENT, memory::AllocatorVoice<>, Size
	>;

	using String = tklb::String<HeapBuffer<char>>;
	using PathString = tklb::String<HeapBuffer<char>>;	///< Non optional string used for locations, maybe replaceable with a unique_ptr or something
	using IdString = tklb::StackString<12>;				///< Non optional string used to id things

} } // namespace vae::core

#endif // _VAE_TYPES
