#ifndef TKLBZ_MEMORY
#define TKLBZ_MEMORY

#include <stddef.h>
#include <utility>

#if !defined(TKLB_CUSTOM_MALLOC) && !defined(TKLB_CUSTOM_FREE)
	#include <stdlib.h>
#endif

#if !defined(TKLB_NO_SIMD) || defined(TKLB_ALIGNED_MEM)
	#include "../external/xsimd/include/xsimd/config/xsimd_config.hpp"
#endif

namespace tklb {
	/**
	 * Wraps allocation so using a custom memory manager
	 * is an option
	 */
	namespace memory {
		/**
		 * @brief Standart allocate function
		 */
		void* allocate(size_t size) noexcept {
			#ifdef TKLB_CUSTOM_MALLOC
				return TKLB_CUSTOM_MALLOC(size);
			#else
				return malloc(size);
			#endif
		}

		/**
		 * @brief Free memory
		 */
		void deallocate(void* ptr) noexcept {
			#ifdef TKLB_CUSTOM_FREE
				TKLB_CUSTOM_FREE(ptr);
			#else
				free(ptr);
			#endif
		}

		void deallocateAligned(void* ptr) noexcept {
			#if !defined(TKLB_NO_SIMD) || defined(TKLB_ALIGNED_MEM)
				// Get the orignal allocation address to free the memory
				deallocate(*(reinterpret_cast<void**>(ptr) - 1));
			#else
				deallocate(ptr);
			#endif
		}

		/**
		 * @brief Allocate aligned if simd is enabled.
		 * Does a normal allocation otherwise.
		 */
		void* allocateAligned(size_t bytes) noexcept {
			#if !defined(TKLB_NO_SIMD) || defined(TKLB_ALIGNED_MEM)
				// TODO replace this
				// This is the aligned allocation routine from xsimd
				// but using the internal allocate function instead
				void* res = 0;
				void* ptr = allocate(bytes + XSIMD_DEFAULT_ALIGNMENT);
				if (ptr != 0 && XSIMD_DEFAULT_ALIGNMENT != 0)
				{
					// some evil bitwise magic to align to the next block
					res = reinterpret_cast<void*>(
						(reinterpret_cast<size_t>(ptr) &
						~(size_t(XSIMD_DEFAULT_ALIGNMENT - 1))) +
						XSIMD_DEFAULT_ALIGNMENT
					);
					// store away the orignal address needed to free the memory
					*(reinterpret_cast<void**>(res) - 1) = ptr;
				}
				return res;
			#else
				return allocate(bytes);
			#endif
		}

		/**
		 * @brief Allocate and construct object
		 * @param args Arguments passed to class contructor
		 */
		template <class T, typename ... Args>
		T* create(Args&& ... args) {
			void* ptr = allocate(sizeof(T));
			if (ptr != nullptr) {
				T* test = new (ptr) T(std::forward<Args>(args)...);
			}
			return ptr;
		}

		/**
		 * @brief Destroy the object and dispose the memory
		 */
		template <class T>
		void dispose(T* ptr) {
			if (ptr != nullptr) {
				ptr->~T();
				deallocate(ptr);
			}
		}
	}
}

// Wrapped in macros so information about where the allocations happened
// can be gathered later on

#define TKLB_MALLOC(size)  ::tklb::memory::allocate(size)
#define TKLB_FREE(ptr) ::tklb::memory::deallocate(ptr)
#define TKLB_MALLOC_ALIGNED(size)  ::tklb::memory::allocateAligned(size)
#define TKLB_FREE_ALIGNED(ptr) ::tklb::memory::deallocateAligned(ptr)
#define TKLB_NEW(T, ...) ::tklb::memory::create<T>(__VA_ARGS__)
#define TKLB_DELETE(T, ptr) ::tklb::memory::dispose<T>(ptr)

#endif // TKLB_MEMORY
