#ifndef TKLBZ_MEMORY_MANAGER
#define TKLBZ_MEMORY_MANAGER


/**
 * Fixed memory
 */
namespace tklb {
	namespace memoryManager {
		const unsigned int CustomSize = 1024 * 1024 * 300;
		unsigned char* CustomMemory = new unsigned char[CustomSize];
		unsigned int Allocated = 0;

		void* customMalloc(unsigned int size) {
			if (size < sizeof(unsigned int)) {
				size = sizeof(unsigned int); // min block size
			}
			size += sizeof(unsigned int); // add space allocated size
			unsigned int* mem = reinterpret_cast<unsigned int*>(CustomMemory);
			for (unsigned int i = 0; i < CustomSize / sizeof(unsigned int) -1;) {
				if (mem[i] == 0) {
					if (size <= mem[i + 1] || mem[i + 1] == 0) {
						mem[i] = size;
						Allocated += size;
						return &(mem[i + 1]);
					}
				}
				i += mem[i];
			}
			return nullptr;
		}

		void customFree(void* ptr) {
			unsigned int* index = reinterpret_cast<unsigned int*>(ptr);
			unsigned int size = *(index - 1);
			*(index - 1) = 0;
			*index = size; // TODO check if the next block is free too
			Allocated -= size;
		}
	}
}

#define TKLB_CUSTOM_MALLOC(size) ::tklb::memoryManager::customMalloc(size)
#define TKLB_CUSTOM_FREE(ptr) ::tklb::memoryManager::customFree(ptr)

#endif
