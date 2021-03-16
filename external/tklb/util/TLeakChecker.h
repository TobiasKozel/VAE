#ifndef TKLB_LEAKCHECKER
#define TKLB_LEAKCHECKER

#include <stddef.h>
#include <stdlib.h>
#include <cassert>
#include <cstring>

#ifdef malloc
	#undef malloc
	#undef free
	#undef realloc
#endif

#ifndef TKLB_MAGIC_NUMBER
	#define TKLB_MAGIC_NUMBER 123456789
#endif

namespace tklb {

// Current allocations
static size_t allocationCount = 0;
// corrupted allocations
static size_t curruptions = 0;

static size_t allocationsTotal = 0;

struct MallocInfo {
	const char* file;
	int line;
	size_t size;
	size_t allocation;
};

void* tklbMalloc(const size_t size, const char* file, int line) {
#ifndef TKLB_LEAKCHECKER_NO_INFO
	if (size == 0) { return nullptr; } // We add bytes, so this is needed
	// Make space for the info struct and a magic number at the end
	MallocInfo* ptr = reinterpret_cast<MallocInfo*>(
		malloc(size + sizeof(MallocInfo) + sizeof(int))
	);
	if (!ptr) { return nullptr; }
	allocationCount++;
	allocationsTotal++;
	ptr->file = file;
	ptr->line = line;
	ptr->size = size;
	ptr->allocation = allocationCount;

	int* magicNumber = reinterpret_cast<int*>(
		reinterpret_cast<unsigned char*>(ptr) + size + sizeof(MallocInfo)
	);
	(*magicNumber) = TKLB_MAGIC_NUMBER;

	return ptr + 1;
#else
	void* ret = malloc(size);
	if (ret) {
		allocationCount++;
		allocationsTotal++;
	}
	return ret;
#endif
}

void* tklbCalloc(const size_t size, const size_t typeSize, const char* file, int line) {
	void* ret = tklbMalloc(size, file, line);
	if (ret) {
		memset(ret, 0, size);
	}
	return ret;
}

void tklbFree(void* ptr, const char* file, int line) {
	if (ptr == nullptr) { return; }
#ifndef TKLB_LEAKCHECKER_NO_INFO
	MallocInfo* info = reinterpret_cast<MallocInfo*>(ptr) - 1;

	int* magicNumber = reinterpret_cast<int*>(
		reinterpret_cast<unsigned char*>(info) + info->size + sizeof(MallocInfo)
	);
	if ((*magicNumber) != TKLB_MAGIC_NUMBER) {
		// Magic number was overwritten
		curruptions++;
		assert(false);
	}
	free(info);
#else
	free(ptr);
#endif
	if (allocationCount == 0) {
		assert(false);
	}
	allocationCount--;
}

void* tklbRealloc(void* ptr, size_t size, const char* file, int line) {
	MallocInfo* info = reinterpret_cast<MallocInfo*>(ptr);
	void* ptr2 = tklbMalloc(size, file, line);
	if (ptr2 == nullptr) { return ptr; }
	if (ptr == nullptr) {
		return ptr2;
	}
#ifndef TKLB_LEAKCHECKER_NO_INFO
	memcpy(ptr2, ptr, info->size < size ? info->size : size);
#else
	memcpy(ptr2, ptr, size);
#endif
	tklbFree(ptr, file, line);
	return ptr2;
}

}

// TODO there seem to be some allocations missing
#ifndef TKLB_LEAKCHECKER_DISARM
/**
 * http://stevehanov.ca/blog/?id=10
 *
 */
void* operator new(size_t size) {
	return tklb::tklbMalloc(size, nullptr, 1337);
}

void* operator new[](size_t size) {
	return tklb::tklbMalloc(size, nullptr, 1337);
}

void operator delete(void* ptr, size_t size) noexcept {
	tklb::tklbFree(ptr, nullptr, 0);
}

void operator delete(void* ptr) noexcept {
	tklb::tklbFree(ptr, nullptr, 0);
}

void operator delete[](void* ptr) noexcept {
	tklb::tklbFree(ptr, nullptr, 0);
}

#define malloc(size)      tklb::tklbMalloc (     size, __FILE__, __LINE__)
#define free(ptr)         tklb::tklbFree   (ptr,       __FILE__, __LINE__)
#define realloc(ptr,size) tklb::tklbRealloc(ptr, size, __FILE__, __LINE__)
#endif

#endif // TKLB_LEAKCHECKER
