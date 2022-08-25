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

#ifdef VAE_NO_STDLIB
	#define TKLB_NO_STDLIB
#endif

#ifdef VAE_IMPL
	#define TKLB_IMPL
#endif

#ifdef VAE_RELEASE
	#define TKLB_RELEASE
#endif

#include "../../external/tklb/src/util/TAssert.h"
#include "../../external/tklb/src/util/TLogger.hpp"
#include "../../external/tklb/src/util/TProfiler.hpp"
#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../external/tklb/src/types/TString.hpp"
#include "../../external/tklb/src/types/TMutex.hpp"

namespace vae { namespace core {
	namespace memory {
		template <class T, class NAME>
		using DefaultAllocator = tklb::DefaultAllocator<T, NAME>;

		constexpr size_t DEFAULT_ALIGNMENT = tklb::DEFAULT_ALIGNMENT;

		template <
			typename T,
			size_t ALIGNMENT,
			class ALLOCATOR,
			typename SIZE
		>
		using HeapBuffer = tklb::HeapBuffer<T, ALIGNMENT, ALLOCATOR, SIZE>;

		template<typename T, typename Handle, class ALLOCATOR>
		using HandleBuffer = tklb::HandleBuffer<T, Handle, 8, ALLOCATOR>;

	}

	using Lock = tklb::Mutex::Lock;
	using Mutex = tklb::Mutex;

} } // vae::core

#define VAE_DEBUG_EVENT(msg, ...) // TODO
#define VAE_DEBUG_VOICES(message, ...) // TODO

#endif // _VAE_TKLB
