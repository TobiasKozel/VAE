/**
 * @file vae_tklb.hpp
 * @author Tobias Kozel
 * @brief All tklb deps are included and controlled from here
 * @version 0.1
 * @date 2022-08-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _VAE_TKLB
#define _VAE_TKLB

#ifdef VAE_NO_SIMD
	#define TKLB_NO_SIMD
#endif

#ifdef VAE_NO_STDIO
	#define TKLB_NO_STDIO
#endif

#ifdef VAE_IMPL
	#define TKLB_IMPL
#endif

#include "../../external/tklb/src/util/TProfiler.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../external/tklb/src/types/TString.hpp"

namespace vae { namespace core { namespace memory {
	template <class T, class NAME>
	using DefaultAllocator = tklb::DefaultAllocator<T, NAME>;

	constexpr size_t DEFAULT_ALIGNMENT = tklb::DEFAULT_ALIGNMENT;

	template <
		typename T,
		size_t ALIGNMENT,
		class ALLOCATOR,
		typename SIZE = unsigned int
	>
	using HeapBuffer = tklb::HeapBuffer<T, ALIGNMENT, ALLOCATOR, SIZE>;

} } } // vae::core::memory

#endif // _VAE_TKLB
