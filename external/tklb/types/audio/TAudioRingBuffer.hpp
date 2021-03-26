#ifndef TKLBZ_AUDIORINGBUFFER
#define TKLBZ_AUDIORINGBUFFER

#include "./TAudioBuffer.hpp"

namespace tklb {
	template <typename T>
	class AudioRingBufferTpl : public AudioBufferTpl<T> {
		using uint = unsigned int;
		uint mHead = 0;
		uint mTail = 0;

	public:
		AudioRingBufferTpl(const uint length, const int channels)
			: AudioBufferTpl(length, channels) { }

		/**
		 * @brief Puts a number of elements in the array provided
		 */
		uint peek(AudioBufferTpl<T>& out, uint elements, uint offset = 0) {
			if (offset < 0) { return 0; } // only use positive offsets
			const uint head = mHead - offset; // Offset the head
			if (elements > head) {
				elements = head; // Clamp the elements to peek to the elements in the buffer
			}
			if (elements > 0) {
				uint tailStart = mTail - head; // This should always be negative when the offset is 0
				if (tailStart < 0) {
					tailStart += size(); // So move it back
				}
				const uint spaceLeft = size() - tailStart;
				if (spaceLeft < elements) {
					// Means it wraps around and split in two moves
					out.set(this, spaceLeft, tailStart);
					out.set(this, (elements - spaceLeft), 0, spaceLeft);
				}
				else {
					// Enough buffer left and can be done in one step
					out.set(this, elements, tailStart);
				}
			}
			return elements;
		}

		/**
		 * @brief Pops a number of elements and puts them in the buffer provided
		 */
		uint pop(AudioBufferTpl<T>& out, const uint elements) {
			const uint elementsOut = peek(out, elements);
			mHead -= elementsOut; // Move the head back, can't exceed bounds since it was clamped in peek
			return elementsOut;
		}

		/**
		 * @brief Adds a number of elements
		 */
		uint push(AudioBufferTpl<T>& in, uint elements) {
			const uint spaceLeftHead = size() - mHead; // Space left before exceeding upper buffer bounds
			if (elements > spaceLeftHead) {
				/**
				 * Clamp the elements added to the buffer to it's bounds
				 * This ring buffer will stop adding elements if that's the case instead of wrapping around
				 */
				elements = spaceLeftHead;
			}
			if (elements > 0) {
				const uint spaceLeftTail = size() - mTail;
				if (spaceLeftTail < elements) {
					// We'll need to wrap around since there's not enough space left
					set(in, spaceLeftTail, 0, mTail);
					set(in, (elements - spaceLeftTail), spaceLeftTail);
					mTail = elements - spaceLeftTail;
				}
				else { // If there's enough space left we can just copy the buffer starting at the tail index
					set(in, elements, 0, nTail);
					// If we end up taking all space left for the tail, move it back to the start, otherwise move it forward
					mTail = (spaceLeftTail == elements) ? 0 : mTail + elements;
				}
				mHead += elements; // Move the head forward, can't exceed the bounds since we clamped it
			}
			return elements;
		}

		/**
		 * @breif Returns how many more elements the buffer can hold
		 */
		uint free() const { return size() - mHead; }

		/**
		 * @brief Returns how many elements are in the buffer
		 */
		uint filled() const { return mHead; }
	};
}
#endif
