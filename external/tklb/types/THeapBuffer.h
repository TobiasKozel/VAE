#ifndef TKLB_HEAPBUFFER
#define TKLB_HEAPBUFFER

#include "../util/TNoCopy.h"
#include "../util/TAssert.h"

#include <cmath>
#include <cstring>
#include <memory>

namespace tklb {

/**
 * Basically a bad std::vector which can also work with
 * foreign memory
 */
template <typename T, class Allocator = std::allocator<T>>
class HeapBuffer {
#if TKLB_HEAP_DEBUG_SIZE > 0
	T DEBUF_BUF[TKLB_HEAP_DEBUG_SIZE];
#endif
	T* mBuf = nullptr;
	size_t mSize = 0; // size of elements requested
	size_t mRealSize = 0; // the actually allocated size
	size_t mGranularity; // the space actually allocated will be a multiple of this
	bool mInjected = false; // True if the memory doesn't belong to this instance
	TKLB_ASSERT_STATE(bool IS_CONST)
	Allocator allocator;

public:
	// TODO handle copying
	TKLB_NO_COPY(HeapBuffer)

	/**
	 * @param size Size in elements of the buffer
	 * @param granularity How big the real allocated chunks are
	 */
	HeapBuffer(const size_t size = 0, const size_t granularity = 1024 / sizeof(T)) {
		setGranularity(granularity);
		if (size != 0) { resize(0); }
	}

	~HeapBuffer() {
		resize(0);
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
	 * Using non const accessors will cause assertions
	 * @param mem Non modifyable memory to use
	 * @param size Size of the memory in elements
	 * @param realSize The actual size if it's chunk allocated
	 */
	void inject(const T* mem, const size_t size, const size_t realSize = 0) {
		inject(const_cast<T*>(mem), size, realSize);
		TKLB_ASSERT_STATE(IS_CONST = true)
	}

	void setGranularity(const size_t granularity) {
		mGranularity = std::min(1u, granularity);
	}

	T* data() {
		// Don't use non const access when using injected const memory
#if TKLB_HEAP_DEBUG_SIZE > 0
		memcpy(DEBUF_BUF, mBuf, sizeof(T) * std::min((size_t) 100, mSize));
#endif
		TKLB_ASSERT(!IS_CONST)
		return mBuf;
	}

	const T* data() const {
		return mBuf;
	}

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

	void reserve(const size_t size) {
		if (size < mRealSize) { return; }
		const size_t temp = mSize;
		resize(size);
		mSize = temp;
	}

	/**
	 * Resize the buffer
	 * If the memory is borrowed it will become unique and owned by this instance
	 * as soon as an allocation happens
	 * @param size The new size
	 * @param downsize Whether to downsize and reallocate
	 */
	T* resize(const size_t size, const bool downsize = true) {
		const size_t chunked = mGranularity * std::ceil(size / double(mGranularity));

		if (size == 0) {
			if (!mInjected && mRealSize > 0) { allocator.deallocate(mBuf, mRealSize); }
			mBuf = nullptr;
			mRealSize = 0;
		} else if (chunked != mRealSize) {
			T* temp = nullptr;
			if (chunked > mRealSize) { // Size up
				temp = allocator.allocate(chunked);
				if (mBuf != nullptr) {
					memcpy(temp, mBuf, mSize * sizeof(T));
					memset(temp + mSize, 0, (chunked - mSize) * sizeof(T));
				}
			} else if(downsize) { // size down
				temp = allocator.allocate(chunked);
				if (mBuf != nullptr) {
					memcpy(temp, mBuf, chunked * sizeof(T));
				}
			}
			if (temp != nullptr) { // an allocation occured
				if (!mInjected && mRealSize > 0) {
					allocator.deallocate(mBuf, mRealSize);
				}
				mInjected = false; // we own the memory now
				TKLB_ASSERT_STATE(IS_CONST = false)
				mBuf = temp;
			}
		}
		mRealSize = chunked;
		mSize = size;
		return mBuf;
	}

	void push(const T& object) {
		if (mRealSize < mSize + 1) {
			resize(mSize + 1);
			mBuf[mSize] = object;
		} else {
			mBuf[mSize] = object;
			mSize++;
		}
	}

	bool remove(const size_t index) {
		if (mSize <= index) { return false; }
		if (index != mSize - 1) {
			mBuf[index] = mBuf[mSize - 1]; // fill the gap with the last element
		}
		mSize--;
		return true;
	}

	void remove(const T& object) {
		for (size_t i = 0; i < mSize; i++) {
			if (mBuf[i] == object) {
				remove(i);
			}
		}
	}

	void construct() {
		for (size_t i = 0; i < mRealSize; i++) {
			allocator.construct(mBuf + i);
		}
	}

	size_t size() const { return mSize; }

};

} // namespace

#endif // TKLB_HEAPBUFFER
