#ifndef TKLBZ_MEMORY_MANAGER
#define TKLBZ_MEMORY_MANAGER

#include "./TAssert.h"

/**
 * Fixed memory
 */
namespace tklb {
	/**
	 * @brief Extremely basic Memorymanager which works with a preallocated
	 * chunk of memory. Fragmentation is probably quite an issue.
	 */
	namespace memoryManager {
		using Size = unsigned int;
		const Size CustomSize = 1024 * 1024 * 300;
		unsigned char* CustomMemory = new unsigned char[CustomSize];
		Size Allocated = 0; // Keep track of total allocations

		void* customMalloc(Size size) {
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

		void customFree(void* ptr) {
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
	}
}

#define TKLB_CUSTOM_MALLOC(size) ::tklb::memoryManager::customMalloc(size)
#define TKLB_CUSTOM_FREE(ptr)    ::tklb::memoryManager::customFree(ptr)

#endif
