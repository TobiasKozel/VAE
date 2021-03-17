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
		resize(source.size());
		memcpy(mBuf, source.data(), size * sizeof(T));
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
		mGranularity = std::min((size_t) 1, granularity);
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

	void reserve(const size_t size) {
		if (size < mRealSize) { return; }
		const size_t temp = mSize;
		resize(size);
		mSize = temp;
	}

	/**
	 * @brief Resize the buffer
	 * If the memory is borrowed it will become unique and owned by this instance
	 * as soon as an allocation happens
	 * @param size The new size
	 * @param downsize Whether to downsize and reallocate
	 */
	T* resize(const size_t size, const bool downsize = true) {
		const size_t chunked = mGranularity * std::ceil(size / double(mGranularity));

		if (size < mSize) {
			for (size_t int i = size; i < mSize; i++) {
				allocator.destroy(mBuf[i]);
			}
		}

		if (size == 0) {
			if (!mInjected && mRealSize > 0) {
				allocator.deallocate(mBuf, mRealSize);
			}
			mBuf = nullptr;
			mRealSize = 0;
			mInjected = false;
			TKLB_ASSERT_STATE(IS_CONST = false)
		} else if (chunked != mRealSize) {
			T* temp = nullptr;
			if (chunked > mRealSize) { // Size up
				temp = allocator.allocate(chunked);
				if (mBuf != nullptr) {
					memcpy(temp, mBuf, mSize * sizeof(T)); // copy existing
					for (size_t i = mSize; i < size; i++) {
						allocator.construct(temp[i]); // construct the new
					}
				}
			} else if(downsize) { // size down
				temp = allocator.allocate(chunked);
				if (mBuf != nullptr) {
					memcpy(temp, mBuf, size * sizeof(T)); // copy existing
				}
			}

			if (temp != nullptr) { // an allocation occured, so clean up
				if (!mInjected && mRealSize > 0) {
					allocator.deallocate(mBuf, mRealSize);
				}
				mInjected = false; // we definetly own the memory now
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

	void pop() {
		// TODO
	}

	/**
	 * @brief Removes the object at a given index and fills the gap with another
	 */
	bool remove(const size_t index) {
		if (mSize <= index) { return false; }
		if (index != mSize - 1) {
			allocator.destroy(mBuf[index]);
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

	size_t size() const { return mSize; }

};

} // namespace

#endif // TKLB_HEAPBUFFER
