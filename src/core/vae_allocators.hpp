/**
 * @file vae_allocators.hpp
 * @author Tobias Kozel
 * @brief Allocator types used are defined here
 * @details We use different allocators with their own names so they can be tracked in the profiler
 * @version 0.1
 * @date 2022-08-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _VAE_ALLOCATORS
#define _VAE_ALLOCATORS

#include "../wrapped/vae_tklb.hpp"

namespace vae { namespace core { namespace memory {
	struct FsAllocatorName		{ static constexpr const char* Name = "FS Allocator";		};
	struct MainAllocatorName	{ static constexpr const char* Name = "Main Allocator";		};
	struct AudioAllocatorName	{ static constexpr const char* Name = "Audio Allocator";	};
	struct ScratchAllocatorName	{ static constexpr const char* Name = "Scratch Allocator";	};
	struct AllocatorEmitterName	{ static constexpr const char* Name = "Emitter Allocator";	};

	template <class T = unsigned char> using AllocatorFS		= DefaultAllocator<T, FsAllocatorName>;
	template <class T = unsigned char> using AllocatorMain		= DefaultAllocator<T, MainAllocatorName>;
	template <class T = unsigned char> using AllocatorAudio		= DefaultAllocator<T, AudioAllocatorName>;
	template <class T = unsigned char> using AllocatorScratch	= DefaultAllocator<T, ScratchAllocatorName>;
	template <class T = unsigned char> using AllocatorEmitter	= DefaultAllocator<T, AllocatorEmitterName>;
} } } // vae::core::memory

#endif // _VAE_ALLOCATORS
