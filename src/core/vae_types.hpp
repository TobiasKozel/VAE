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
#include "./vae_allocators.hpp"
#include "../wrapped/vae_tklb.hpp"

#ifndef VAE_NO_AUDIO_THREAD
	#include <mutex>
#else
	#include "../../external/tklb/src/types/TSpinLock.hpp"
#endif // !VAE_NO_AUDIO_THREAD

namespace vae { namespace core {
	using Uchar = unsigned char;
	using Uint = unsigned int;
	using Real = float;
	#ifndef VAE_USE_PROFILER
		// vae_profiler.hpp provides wrapped versions for tracking
		#ifndef VAE_NO_AUDIO_THREAD
			using Mutex	= std::mutex;
			using Lock	= std::unique_lock<Mutex>;
		#else
			using Mutex	= tklb::SpinLock;
			using Lock	= tklb::LockGuard<Mutex>;
		#endif // !#ifndef VAE_NO_AUDIO_THREAD
	#endif // !VAE_USE_PROFILER

	/**
	 * @brief Buffer type to hold all the audio data meant for playback
	 */
	using AudioBuffer = tklb::AudioBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, memory::DEFAULT_ALIGNMENT, memory::AllocatorAudio<unsigned char>
		>
	>;

	/**
	 * @brief Buffer type to do all the work like mixing, filtering and so on
	 */
	using ScratchBuffer = tklb::AudioBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, memory::DEFAULT_ALIGNMENT, memory::AllocatorScratch<unsigned char>
		>
	>;

	using RingBuffer = tklb::AudioRingBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, memory::DEFAULT_ALIGNMENT, memory::AllocatorScratch<unsigned char>
		>
	>;

	using SampleIndex = AudioBuffer::Size;	// Time stored in samples

	#ifdef VAE_RELEASE
		using NameString = tklb::StackString<0>;			///< Used for debug names
	#else
		using NameString = tklb::StackString<16>;			///< Used for debug names
	#endif // VAE_RELEASE
		using String = tklb::String<HeapBuffer<char>>;
		using PathString = tklb::String<HeapBuffer<char>>;	///< Non optional string used for locations, maybe replaceable with a unique_ptr or something
		using IdString = tklb::StackString<16>;				///< Non optional string used to id things

} } // namespace vae::core


#endif // _VAE_TYPES
