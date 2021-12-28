#ifndef _VAE_ALLOCATORS
#define _VAE_ALLOCATORS

#include <cstdlib>
#include <cstddef>
#include <new>
#include <cstddef>
#include <memory>
#include <limits>
#include "../wrapped/vae_profiler.hpp"

namespace vae { namespace core { namespace memory {
	/**
	 * @brief Allocator used for all heapbuffers in VAE
	 * @tparam T
	 */
	template <class T, class NAME>
	struct Allocator {
		typedef T value_type;

		Allocator() = default;
		Allocator(const Allocator&) = default;
		template <class U, class J >
		Allocator(const Allocator<U, J>&) { }

		T* allocate(std::size_t n) noexcept {
			if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
				return nullptr;
			}

			if (auto ptr = static_cast<T*>(std::malloc(n * sizeof(T)))) {
				VAE_PROFILER_MALLOC_L(ptr, n * sizeof(T), NAME::name)
				return ptr;
			}

			return nullptr;
		}

		void deallocate(T* ptr, std::size_t n) noexcept {
			VAE_PROFILER_FREE_L(ptr, NAME::name)
			std::free(ptr);
		}
	};

	/**
	 * @brief I don't even know what this does, but it has to be here for some classes using
	 */
	template <class T, class U, class J >
		bool operator==(const Allocator<T, J>&, const Allocator<U, J>&) {
		return true;
	}

	template <class T, class U, class J >
		bool operator!=(const Allocator<T, J>&, const Allocator<U, J>&) {
		return false;
	}

	/**
	 * @brief This is here to get a name into the allocator without writing the whole thing again
	 */
	struct FsAllocatorName		{ static constexpr const char* name = "FS Allocator";		};
	struct MainAllocatorName	{ static constexpr const char* name = "Main Allocator";		};
	struct AudioAllocatorName	{ static constexpr const char* name = "Audio Allocator";	};
	struct ScratchAllocatorName	{ static constexpr const char* name = "Scratch Allocator";	};

	template <class T> using AllocatorFS		= Allocator<T, FsAllocatorName>;
	template <class T> using AllocatorMain		= Allocator<T, MainAllocatorName>;
	template <class T> using AllocatorAudio		= Allocator<T, AudioAllocatorName>;
	template <class T> using AllocatorScratch	= Allocator<T, ScratchAllocatorName>;

} } } // vae::core::memory

#endif // _VAE_ALLOCATORS
