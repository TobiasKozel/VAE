#ifndef TKLBZ_MEMORY
#define TKLBZ_MEMORY

#include <stddef.h>
#include <utility>

#ifndef TKLB_MEM_CUSTOM_MALLOC
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

	#ifndef TKLB_MEM_CUSTOM_MALLOC
		void* allocate(size_t size) noexcept {
			return malloc(size);
		}

		void* reallocate(void* ptr, size_t size) noexcept {
			return realloc(ptr, size);
		}

		void deallocate(void* ptr) noexcept {
			free(ptr);
		}
	#else
		void* allocate(size_t size) noexcept;
		void* reallocate(void* ptr, size_t size) noexcept;
		void deallocate(void* ptr) noexcept;
	#endif // TKLB_MEM_CUSTOM_MALLOC

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
				if (ptr != 0 && XSIMD_DEFAULT_ALIGNMENT != 0) {
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
		 * @brief Besic memcpy
		 */
		inline void copy(void* dst, const void* src, const size_t size) {
			auto source = reinterpret_cast<const unsigned char*>(src);
			auto destination = reinterpret_cast<unsigned char*>(dst);
			for (size_t i = 0; i < size; i++) {
				destination[i] = source[i];
			}
		}

		inline void set(void* ptr, const unsigned char c, size_t size) {
			auto pointer = reinterpret_cast<unsigned char*>(ptr);
			for (size_t i = 0; i < size; i++) {
				pointer[i] = c;
			}
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
			return reinterpret_cast<T*>(ptr);
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
#define TKLB_REALLOC(ptr, size) ::tklb::memory::reallocate(ptr, size)
#define TKLB_MALLOC_ALIGNED(size)  ::tklb::memory::allocateAligned(size)
#define TKLB_FREE_ALIGNED(ptr) ::tklb::memory::deallocateAligned(ptr)
#define TKLB_NEW(T, ...) ::tklb::memory::create<T>(__VA_ARGS__)
#define TKLB_DELETE(ptr) ::tklb::memory::dispose(ptr)

#endif // TKLB_MEMORY
