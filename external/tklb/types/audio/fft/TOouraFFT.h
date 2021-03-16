#ifndef TKLB_FFT_OOURA
#define TKLB_FFT_OOURA


#include <vector>
#include "../TAudioBuffer.h"
#include "../../../external/Ouura.h"

namespace tklb {

class FFTOoura {
	using uchar = unsigned char;
	using uint = unsigned int;

	uint mSize;
	// No idea what this is
	AudioBuffer::Buffer<int> mIp;
	// or this, prolly lookup tables
	AudioBufferDouble mW;
	AudioBufferDouble mBuffer;

public:
	FFTOoura(uint size = 0) {
		if (size == 0) { return; }
		resize(size);
	}

	void resize(uint size) {
		if (size == mSize) { return; }
		mIp.resize(2 + int(std::sqrt(double(size))));
		mW.resize(size / 2);
		mBuffer.resize(size);
		mSize = size;

		const int size4 = size / 4;
		ooura::makewt(size4, mIp.data(), mW[0]);
		ooura::makect(size4, mIp.data(), mW[0] + size4);
	}

	/**
	 * @brief timedomain to frequency domain
	 * @param input Input buffer, validSize needs to be a multiple of 2
	 * @param output Output buffer, must have 2 channel for real and imaginary. Half the length of thie input buffer
	 */
	template <typename T>
	void forward(const AudioBufferTpl<T>& input, AudioBufferTpl<T>& result) {
		TKLB_ASSERT(input.validSize() % mSize == 0) // Only multiples os the chunk size
		uint processed = 0;
		while (processed < input.validSize()) {
			const uint processedHalf = processed / 2;
			T* real = result[0] + processedHalf;
			T* imaginary = result[1] + processedHalf;
			const T* in = input[0] + processed;
			/**
			 * converts or copies the data
			 * we shouldn't use the original buffer since
			 * mBuffer gets written to by ooura
			 */
			mBuffer.set(in, mSize);
			ooura::rdft(mSize, +1, mBuffer[0], mIp.data(), mW[0]);

			// Convert back to split-complex
			{
				// TODO make this mess readable
				double* b = mBuffer[0];
				double* bEnd = b + mSize;
				T* r = real;
				T* i = imaginary;
				while (b != bEnd) {
					*(r++) = (*(b++));
					*(i++) = (-(*(b++)));
				}
			}
			const size_t size2 = mSize / 2;
			// real[size2] = -imaginary[0]; // this doens't make any sense
			// imaginary[0] = 0.0;
			// imaginary[size2] = 0.0; // this doesn't either
			processed += mSize;
		}

	}

	/**
	 * @brief Frequency domain back to time domain
	 * @param input Buffer with 2 channels. channel 0 for real and 1 for imaginary
	 * @param result Single channel output buffer. Needs to be twice the size of the imput buffer
	 */
	template <typename T>
	void back(const AudioBufferTpl<T>& input, AudioBufferTpl<T>& result) {
		TKLB_ASSERT(input.validSize() % mSize == 0) // Only multiples os the chunk size
		const T* real = input[0];
		const T* imaginary = input[1];
		T* out = result[0];
		uint processed = 0;
		while (processed < input.validSize()) {
			{
				// TODO make this mess readable
				double* b = mBuffer[0];
				double* bEnd = b + mSize;
				const uint processedHalf = processed / 2;
				const T* r = real + processedHalf;
				const T* i = imaginary + processedHalf;
				while (b != bEnd) {
					*(b++) = (*(r++));
					*(b++) = -(*(i++));
				}
				mBuffer[0][1] = real[mSize / 2];
			}

			ooura::rdft(int(mSize), -1, mBuffer[0], mIp.data(), mW[0]);

			const T volume = 2.0 / T(mSize);
			if (std::is_same<T, double>::value) {
				mBuffer.multiply(volume); // scale the output
				mBuffer.put(reinterpret_cast<double*>(out + processed), mSize);
			} else {
				// or the old fashioned way if we need to convert sample types anyways
				const double* buf = mBuffer[0];
				for (uint i = 0; i < mSize; i++) {
					out[i + processed] = buf[i] * volume;
				}
			}
			processed += mSize;
		}
	}
};

} // namespace

#endif // TKLB_FFT_OOURA
