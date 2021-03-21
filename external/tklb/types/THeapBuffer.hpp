#ifndef TKLB_HEAPBUFFER
#define TKLB_HEAPBUFFER

#include "../util/TAssert.h"
#include "../util/TMemory.hpp"

#include <cmath>
#include <cstring>

namespace tklb {

/**
 * Basically a bad std::vector which can also work with foreign memory.
 * Classes stored inside need to have a default contructor
 */
template <typename T, bool Aligned = false>
class HeapBuffer {
#if TKLB_HEAP_DEBUG_SIZE > 0
	T DEBUF_BUF[TKLB_HEAP_DEBUG_SIZE];
#endif
	T* mBuf = nullptr; // Underlying buffer
	size_t mSize = 0; // size of elements requested
	size_t mRealSize = 0; // the actually allocated size
	size_t mGranularity; // the space actually allocated will be a multiple of this
	bool mInjected = false; // True if the memory doesn't belong to this instance
	bool mError = false;

	// True when the foreign memory is const, only cheked in debug mode
	TKLB_ASSERT_STATE(bool IS_CONST)

	/**
	 * @brief Allocated the exact size requsted and copies existing objects.
	 * Will not call their destructors or constructors!
	 */
	bool allocate(size_t chunk) noexcept {
		T* oldBuf = mBuf;
		if (0 < chunk) {
			T* newBuf = nullptr;
			if (Aligned) {
				newBuf = reinterpret_cast<T*>(
					memory::allocateAligned(chunk * sizeof(T))
				);
			} else {
				newBuf = reinterpret_cast<T*>(
					memory::allocate(chunk * sizeof(T))
				);
			}
			if (newBuf == nullptr) {
				TKLB_ASSERT(false)
				mError = true;
				return false; // ! Allocation failed
			}
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
			if (Aligned) {
				memory::deallocateAligned(oldBuf);
			} else {
				memory::deallocate(oldBuf);
			}
		}

		mInjected = false; // we definetly own the memory now
		TKLB_ASSERT_STATE(IS_CONST = false)
		mRealSize = chunk;
		mError = false;
		return true;
	}

	size_t closestChunkSize(size_t chunk) const {
		return mGranularity * std::ceil(chunk / double(mGranularity));
	}

public:

	/**
	 * @brief Setup the buffer with a size. User has to check if allocation was successful.
	 * @param size Size in elements of the buffer
	 * @param granularity How big the real allocated chunks are
	 */
	HeapBuffer(const size_t size = 0, const size_t granularity = 1024) {
		setGranularity(granularity);
		if (size != 0) { resize(0); }
	}

	/**
	 * @brief Copy the contents of another buffer in
	 * See set()
	 * Failed allocations have to be checked
	 */
	HeapBuffer(const HeapBuffer<T>& source) {
		set(source);
	}

	HeapBuffer(const HeapBuffer*) = delete;
	HeapBuffer(HeapBuffer&&) = delete;
	HeapBuffer& operator= (const HeapBuffer&) = delete;
	HeapBuffer& operator= (HeapBuffer&&) = delete;

	~HeapBuffer() { resize(0); }

	/**
	 * @brief Resizes and copies the contents of the source Buffer
	 * This will do a memcpy,so none of the object
	 * contructors will called
	 */
	bool set(const HeapBuffer<T>& source) {
		setGranularity(source.mGranularity);
		if (mBuf != nullptr) {
			resize(0); // Clear first so no old data gets copied on resize
		}
		if (!resize(source.size())) {
			return false; // ! Allocation failed
		}
		memcpy(mBuf, source.data(), mSize * sizeof(T));
		return true;
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
	 * @return Whether the allocation was succesful
	 */
	bool reserve(const size_t size) {
		if (size  < mRealSize) { return true; }
		return allocate(closestChunkSize(size));
	}

	/**
	 * @brief Resize the buffer
	 * If the memory is borrowed it will become unique and owned by this instance
	 * as soon as an allocation happens
	 * @param size The new size
	 * @param downsize Whether to downsize and reallocate
	 * @return Whether the allocation was successful
	 */
	bool resize(const size_t size, const bool downsize = true) {
		const size_t chunked = closestChunkSize(size);

		if (size < mSize && mBuf != nullptr) { // downsize means destroy objects
			for (size_t i = size; i < mSize; i++) {
				mBuf[i].~T(); // Call destructor
			}
		}

		if (mRealSize < chunked || (downsize && (mRealSize > chunked)) ) {
			// Upsize or downsize + downsize requested
			if (!allocate(chunked)) {
				return false; // ! Allocation failed
			}
		}

		if (mSize < size) { // upsize means construct objects
			for (size_t i = mSize; i < size; i++) {
				T* test = new (mBuf + i) T();
			}
		}

		mSize = size;
		return true;
	}

	bool push(const T& object) {
		size_t newSize = mSize + 1;
		if (mRealSize < newSize) {
			if (allocate(closestChunkSize(newSize))) {
				memcpy(mBuf + mSize, &object, sizeof(T));
			} else {
				return false; // ! Allocation failed
			}
		} else {
			memcpy(mBuf + mSize, &object, sizeof(T));
		}
		mSize = newSize;
		return true;
	}

	/**
	 * @brief Get the last element in the buffer.
	 * Will never shrink the buffer/allocate
	 */
	bool pop(T* object) {
		if (0 == mSize) {
			return false;
		}
		memcpy(object, mBuf + mSize - 1, sizeof(T));
		mSize--;
		return true;
	}

	/**
	 * @brief Removes the object at a given index and fills the gap with another
	 * Will never shrink the buffer/allocate
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

	/**
	 * @brief Returns the amount of elements in the container
	 */
	size_t size() const { return mSize; }

	/**
	 * @brief Returns the real allocated size in elements
	 */
	size_t reserved() const { return mRealSize; }

	/**
	 * @brief Returns true if the most recent allocation failed
	 */
	bool error() const { return mError; }

};

} // namespace

#endif // TKLB_HEAPBUFFER
