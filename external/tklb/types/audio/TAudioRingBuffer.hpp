#ifndef TKLBZ_AUDIORINGBUFFER
#define TKLBZ_AUDIORINGBUFFER

#include "./TAudioBuffer.hpp"

namespace tklb {
	template <typename T>
	class AudioRingBufferTpl : public AudioBufferTpl<T> {
		using uchar = unsigned char;
		using uint = unsigned int;
		using Base = AudioBufferTpl<T>;
		uint mHead = 0;
		uint mTail = 0;

	public:
		AudioRingBufferTpl(const uint length, const uchar channels)
			: AudioBufferTpl<T>(length, channels) { }

		/**
		 * @brief Puts a number of elements in the buffer provided
		 * @param out Destination buffer to store retrieved samples in
		 * @param elements How many elements to retrieve from the ring buffer
		 * @param offsetSrc Where to start in the ringbuffer
		 * @param offsetDst Where to start in the destination buffer
		 * @return How many elements where retrieved
		 */
		uint peek(AudioBufferTpl<T>& out, uint elements, uint offsetSrc = 0, uint offsetDst = 0) {
			const uint head = mHead - offsetSrc; // Offset the head
			if (elements > head) {
				elements = head; // Clamp the elements to peek to the elements in the buffer
			}
			if (elements > 0) {
				uint tailStart = mTail - head; // This should always be negative when the offset is 0
				if (tailStart < 0) {
					tailStart += Base::size(); // So move it back
				}
				const uint spaceLeft = Base::size() - tailStart;
				if (spaceLeft < elements) {
					// Means it wraps around and split in two moves
					out.set(*this, spaceLeft             , tailStart, offsetDst);
					out.set(*this, (elements - spaceLeft), 0        , spaceLeft + offsetDst);
				}
				else {
					// Enough buffer left and can be done in one step
					out.set(*this, elements, tailStart, offsetDst);
				}
			}
			return elements;
		}

		/**
		 * @brief Pops a number of elements in the buffer provided
		 * @param out Destination buffer to store retrieved samples in
		 * @param elements How many elements to retrieve from the ring buffer
		 * @param offsetSrc Where to start in the ringbuffer
		 * @param offsetDst Where to start in the destination buffer
		 * @return How many elements where retrieved
		 */
		uint pop(AudioBufferTpl<T>& out, const uint elements, uint offsetSrc = 0, uint offsetDst = 0) {
			const uint elementsOut = peek(out, elements, offsetSrc, offsetDst);
			mHead -= elementsOut; // Move the head back, can't exceed bounds since it was clamped in peek
			return elementsOut;
		}

		/**
		 * @brief Adds a number of elements
		 * @param in Source buffer
		 * @param elements How many elements to retrieve from the source buffer
		 * @param offsetSrc Where to start in the source buffer
		 * @return How many elements where stored in the ring buffer
		 */
		uint push(AudioBufferTpl<T>& in, uint elements, uint offsetSrc = 0) {
			const uint spaceLeftHead = Base::size() - mHead; // Space left before exceeding upper buffer bounds
			if (elements > spaceLeftHead) {
				/**
				 * Clamp the elements added to the buffer to it's bounds
				 * This ring buffer will stop adding elements if that's the case instead of wrapping around
				 */
				elements = spaceLeftHead;
			}
			if (elements > 0) {
				const uint spaceLeftTail = Base::size() - mTail;
				if (spaceLeftTail < elements) {
					// We'll need to wrap around since there's not enough space left
					Base::set(in, spaceLeftTail             , 0 + offsetSrc            , mTail);
					Base::set(in, (elements - spaceLeftTail), spaceLeftTail + offsetSrc);
					mTail = elements - spaceLeftTail;
				}
				else { // If there's enough space left we can just copy the buffer starting at the tail index
					Base::set(in, elements, offsetSrc, mTail);
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
		uint free() const { return Base::size() - mHead; }

		/**
		 * @brief Returns how many elements are in the buffer
		 */
		uint filled() const { return mHead; }
	};

	typedef AudioRingBufferTpl<float> AudioRingBufferFloat;
	typedef AudioRingBufferTpl<double> AudioRingBufferDouble;

	// Default type
	#ifdef TKLB_SAMPLE_FLOAT
		typedef AudioRingBufferFloat AudioRingBuffer;
	#else
		typedef AudioRingBufferDouble AudioRingBuffer;
	#endif
}
#endif
