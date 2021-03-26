#ifndef TKLBZ_MEMORY_MANAGER
#define TKLBZ_MEMORY_MANAGER

#include "./TAssert.h"
#include "./TMath.hpp"

#define TKLB_MEM_CUSTOM_MALLOC
#include "./TMemory.hpp"

/**
 * Fixed memory
 */
namespace tklb {
	/**
	 * @brief Extremely basic Memorymanager which works with a preallocated
	 * chunk of memory. Fragmentation is probably quite an issue.
	 * [size|data],[size|data],[0|size of free block]
	 * example:
	 * size is stored in a 4 byte int and includes itself
	 * [12|Space of 8 bytes],
	 * [8|space of 4 bytes],
	 * [0,8](4 byte of actual free space),
	 * [0,0](free space till end of buffer)
	 */
	namespace memory {
		using Size = unsigned int;
		const Size CustomSize = 1024 * 1024 * 300;
		unsigned char* CustomMemory = new unsigned char[CustomSize];
		Size Allocated = 0; // Keep track of total allocations

		void* allocate(size_t size) noexcept {
			if (size == 0) { return nullptr; }
			if (size < sizeof(Size)) {
				// min block size since the space will be used when it's free
				size = sizeof(Size);
			}
			size += sizeof(Size); // add space allocated size
			Size* mem = reinterpret_cast<Size*>(CustomMemory);
			for (Size i = 0; i < CustomSize / sizeof(Size) -1;) {
				if (mem[i] == 0) {
					if (size <= mem[i + 1] || mem[i + 1] == 0) {
						mem[i] = size;
						Allocated += size;
						return &(mem[i + 1]); // * Found free spot
					} else {
						// Step over the free area which is too small
						i += mem[i + 1];
					}
				} else {
					i += mem[i]; // Step over the already allocated area
				}
			}
			return nullptr; // ! No memory left
		}

		void deallocate(void* ptr) noexcept {
			if (ptr == nullptr) { return; }
			Size* index = reinterpret_cast<Size*>(ptr);
			Size size = *(index - 1);
			*(index - 1) = 0; // Mark the block as unallocated

			// Save how large is the gap in memory will be
			// TODO check if the next block is free too
			// These blocks should be merged or fragmentation gets worse
			*index = size;
			Allocated -= size;
		}

		void* reallocate(void* ptr, size_t size) noexcept {
			if (ptr == nullptr) { return allocate(size); }

			Size* index = reinterpret_cast<Size*>(ptr);
			const Size oldSize = *(index - 1);
			const Size newSize = size + sizeof(Size);
			if (newSize <= oldSize) {
				if (oldSize <= newSize + sizeof(Size)) {
					// * Don't do anything since there's not enough
					// * space beeing freed to mark a spare block
					return ptr;
				}
				*(index - 1) = newSize;
				*(index + size) = 0; // Mark the start of a new block
				// Mark the size of the new free block
				*(index + newSize) = oldSize - newSize;
				return ptr; // * Down size
			}
			void* newPtr = allocate(size);
			if (newPtr == nullptr) { return nullptr; }
			const Size bytes = min(oldSize, newSize) - sizeof(Size);
			copy(ptr, newPtr, bytes);
			deallocate(ptr);
			return newPtr;
		}
	}
}

#endif
