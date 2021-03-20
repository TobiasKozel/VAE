#ifndef TKLB_HEAPBUFFER
#define TKLB_HEAPBUFFER

#include "../util/TAssert.h"

#include <cmath>
#include <cstring>
#include <memory>

namespace tklb {

/**
 * Basically a bad std::vector which can also work with foreign memory
 */
template <typename T, class Allocator = std::allocator<T>>
class HeapBuffer {
#if TKLB_HEAP_DEBUG_SIZE > 0
	T DEBUF_BUF[TKLB_HEAP_DEBUG_SIZE];
#endif
	Allocator allocator;
	T* mBuf = nullptr;
	size_t mSize = 0; // size of elements requested
	size_t mRealSize = 0; // the actually allocated size
	size_t mGranularity; // the space actually allocated will be a multiple of this
	bool mInjected = false; // True if the memory doesn't belong to this instance

	// True when the foreign memory is const, only cheked in debug mode
	TKLB_ASSERT_STATE(bool IS_CONST)

	/**
	 * @brief Allocated the exact size requsted and copies existing objects.
	 * Will not call their destructors or constructors!
	 */
	void allocate(size_t chunk) {
		T* oldBuf = mBuf;
		if (0 < chunk) {
			T* newBuf = allocator.allocate(chunk);
			if (0 < mSize && oldBuf != nullptr && newBuf != nullptr) {
				// copy existing content
				memcpy(newBuf, oldBuf, mSize * sizeof(T));
			}
			mBuf = newBuf;
		} else {
			mBuf = nullptr;
		}

		if (oldBuf != nullptr && !mInjected && mRealSize > 0) {
			// Get rif of oldbuffer, object destructors were alredy called
			allocator.deallocate(oldBuf, mRealSize);
		}

		mInjected = false; // we definetly own the memory now
		TKLB_ASSERT_STATE(IS_CONST = false)
		mRealSize = chunk;
	}

	size_t closestChunkSize(size_t chunk) const {
		return mGranularity * std::ceil(chunk / double(mGranularity));
	}

public:

	/**
	 * @param size Size in elements of the buffer
	 * @param granularity How big the real allocated chunks are
	 */
	HeapBuffer(const size_t size = 0, const size_t granularity = 1024) {
		setGranularity(granularity);
		if (size != 0) { resize(0); }
	}

	HeapBuffer(const HeapBuffer<T, Allocator>& source) {
		set(source);
	}

	HeapBuffer(const HeapBuffer*) = delete;
	HeapBuffer(HeapBuffer&&) = delete;
	HeapBuffer& operator= (const HeapBuffer&) = delete;
	HeapBuffer& operator= (HeapBuffer&&) = delete;

	~HeapBuffer() { resize(0); }

	/**
	 * @brief Resizes and copies the contents of the source Buffer
	 */
	void set(const HeapBuffer<T, Allocator>& source) {
		setGranularity(source.mGranularity);
		if (mBuf != nullptr) {
			resize(0); // Clear first so no old data gets copied
		}
		resize(source.size());
		memcpy(mBuf, source.data(), mSize * sizeof(T));
	}

	/**
	 * @brief Provide foreign memory to borrow
	 * @param mem Modifyable memory to use
	 * @param size Size of the memory in elements
	 * @param realSize The actual size if it's chunk allocated
	 */
	void inject(T* mem, const size_t size, const size_t realSize = 0) {
		if (!mInjected && mBuf != nullptr) { resize(0); };
		TKLB_ASSERT_STATE(IS_CONST = false)
		mInjected = true;
		mBuf = mem;
		mSize = size;
		mRealSize = realSize == 0 ? size : realSize;
	}

	/**
	 * @brief Provide const foreign memory to use
	 * Using non const accessors will cause assertions in debug mode
	 * @param mem Non modifyable memory to use
	 * @param size Size of the memory in elements
	 * @param realSize The actual size if it's chunk allocated
	 */
	void inject(const T* mem, const size_t size, const size_t realSize = 0) {
		inject(const_cast<T*>(mem), size, realSize);
		TKLB_ASSERT_STATE(IS_CONST = true)
	}

	void setGranularity(const size_t granularity) {
		mGranularity = std::max((size_t) 1, granularity);
	}

	T* data() {
#if TKLB_HEAP_DEBUG_SIZE > 0
		memcpy(DEBUF_BUF, mBuf, sizeof(T) * std::min((size_t) 100, mSize));
#endif
		// Don't use non const access when using injected const memory
		TKLB_ASSERT(!IS_CONST)
		return mBuf;
	}

	const T* data() const { return mBuf; }

	const T& operator[](const size_t index) const {
		return mBuf[index];
	}

	T& operator[](const size_t index) {
		// Don't use non const access when using injected const memory
#if TKLB_HEAP_DEBUG_SIZE > 0
		memcpy(DEBUF_BUF, mBuf, sizeof(T) * std::min((size_t) 100, mSize));
#endif
		TKLB_ASSERT(!IS_CONST)
		return mBuf[index];
	}

	/**
	 * @brief Will make sure the desired space is allocated
	 */
	void reserve(const size_t size) {
		if (size  < mRealSize) { return; }
		allocate(closestChunkSize(size));
	}

	/**
	 * @brief Resize the buffer
	 * If the memory is borrowed it will become unique and owned by this instance
	 * as soon as an allocation happens
	 * @param size The new size
	 * @param downsize Whether to downsize and reallocate
	 */
	T* resize(const size_t size, const bool downsize = true) {
		const size_t chunked = closestChunkSize(size);

		if (size < mSize && mBuf != nullptr) { // downsize means destroy objects
			for (size_t i = size; i < mSize; i++) {
				mBuf[i].~T(); // Call destructor
			}
		}

		if (mRealSize < chunked || (downsize && (mRealSize > chunked)) ) {
			// Upsize or downsize + downsize requested
			allocate(chunked);
		}

		if (mSize < size) { // upsize means construct objects
			for (size_t i = mSize; i < size; i++) {
				T* test = new (mBuf + i) T();
			}
		}

		mSize = size;
		return mBuf;
	}

	void push(const T& object) {
		size_t newSize = mSize + 1;
		if (mRealSize < newSize) {
			allocate(closestChunkSize(newSize));
			memcpy(mBuf + mSize, &object, sizeof(T));
		} else {
			memcpy(mBuf + mSize, &object, sizeof(T));
		}
		mSize = newSize;
	}

	void pop() {
		// TODO
	}

	/**
	 * @brief Removes the object at a given index and fills the gap with another
	 */
	bool remove(const size_t index) {
		if (mSize <= index) { return false; }
		mBuf[index].~T(); // Call destructor
		if (index != mSize - 1) { // fill the gap with the last element
			memcpy(mBuf + index, mBuf + (mSize - 1), sizeof(T));
		}
		mSize--;
		return true;
	}

	bool remove(const T& object) {
		for (size_t i = 0; i < mSize; i++) {
			if (mBuf[i] == object) {
				return remove(i);
			}
		}
		return false;
	}

	size_t size() const { return mSize; }

};

} // namespace

#endif // TKLB_HEAPBUFFER
