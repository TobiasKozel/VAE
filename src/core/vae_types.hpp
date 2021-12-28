/**
 * @file vae_types.hpp
 * @author Tobias Kozel (t.kozel@pm.me)
 * @brief Internal types used across VAE
 * @version 0.1
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _VAE_TYPES
#define _VAE_TYPES

#ifdef VAE_NO_SIMD
	#define TKLB_NO_SIMD
#endif

#ifdef VAE_NO_STDIO
	#define TKLB_NO_STDIO
#endif


#include "../../include/vae/vae.hpp"
#include "../wrapped/vae_profiler.hpp"
#include "./vae_config.hpp"
#include "../../external/glm/glm/glm.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../external/tklb/src/types/TString.hpp"
#include "./vae_allocators.hpp"
#include <memory>

#ifndef VAE_NO_AUDIO_THREAD
	#include <mutex>
#else
	#include "../../external/tklb/src/types/TSpinLock.hpp"
#endif // !VAE_NO_AUDIO_THREAD

namespace vae { namespace core {
	using Uchar = unsigned char;
	using Uint = unsigned int;
	using Vec3 = glm::vec3;

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

	// Buffer used for internal packed structs like Event, Bank Voice etc
	template <class T> using HeapBuffer = tklb::HeapBuffer<T, 0, memory::AllocatorMain<unsigned char>>;

	using AudioBuffer = tklb::AudioBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, tklb::DEFAULT_ALIGNMENT, memory::AllocatorAudio<unsigned char>
		>
	>;


	using ScratchBuffer = tklb::AudioBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, tklb::DEFAULT_ALIGNMENT, memory::AllocatorScratch<unsigned char>
		>
	>;

	using RingBuffer = tklb::AudioRingBufferTpl<
		Sample, tklb::HeapBuffer<
			Sample, tklb::DEFAULT_ALIGNMENT, memory::AllocatorAudio<unsigned char>
		>
	>;

	using SampleIndex = AudioBuffer::Size;	// Time stored in samples

	#ifdef VAE_RELEASE
		using NameString = tklb::StackString<0>;		///< Used for debug names
	#else
		using NameString = tklb::StackString<16>;		///< Used for debug names
	#endif // VAE_RELEASE
		using String = tklb::String<HeapBuffer<char>>;
		using PathString = tklb::String<HeapBuffer<char>>;	///< Non optional string used for locations, maybe replaceable with a unique_ptr or something
		using IdString = tklb::StackString<16>;				///< Non optional string used to id things

} } // namespace vae::core


#endif // _VAE_TYPES
