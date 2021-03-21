#ifndef TKLB_MEMORY
#define TKLB_MEMORY

#include <stddef.h>
#include <stdlib.h>
#include <utility>

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
		void* allocate(size_t bytes) noexcept {
			return malloc(bytes);
		}

		/**
		 * @brief Free memory
		 */
		void deallocate(void* ptr) noexcept {
			free(ptr);
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
		 * @breif Destroy the object and dispose the memory
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

#endif // TKLB_MEMORY
