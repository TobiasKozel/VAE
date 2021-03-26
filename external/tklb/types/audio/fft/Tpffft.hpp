#ifndef TKLBZ_FFT_PFFFT
#define TKLBZ_FFT_PFFFT


#ifdef TKLB_NO_SIMD
	#define PFFFT_SIMD_DISABLE
#endif

#include "../../../external/pffft/pffft.h"
#include "../../../external/pffft/pffft.c"
#include "../../../external/pffft/pffft_common.c"

#include <cmath>
#include <vector>
#include "../TAudioBuffer.hpp"

namespace tklb {

	class FFTpffft {
		using uchar = unsigned char;
		using uint = unsigned int;

		uint mSize;
		PFFFT_Setup* mSetup = nullptr;
		AudioBufferFloat mBuffer; //  Audiobuffer
		AudioBufferFloat mRc; // RealComplexBuffer

	public:
		FFTpffft(uint size = 0) {
			if (size == 0) { return; }
			resize(size);
		}

		~FFTpffft() {
			if (mSetup == nullptr) { return; }
			pffft_destroy_setup(mSetup);
		}

		void resize(uint size) {
			if (mSetup != nullptr) {
				pffft_destroy_setup(mSetup);
			}
			mSetup = pffft_new_setup(size, PFFFT_REAL);
			mBuffer.resize(size);
			mRc.resize(size * 2);
			mSize = size;
		}

		/**
		 * @brief timedomain to frequency domain
		 * @param input Input buffer, validSize needs to be a multiple of 2
		 * @param output Output buffer, must have 2 channel for real and imaginary. Half the length of thie input buffer
		 */
		template <typename T>
		void forward(const AudioBufferTpl<T>& input, AudioBufferTpl<T>& result) {
			uint processed = 0;
			TKLB_ASSERT(input.validSize() % mSize == 0) // Only multiples os the chunk size
			while (processed < input.validSize()) {
				const float* data = nullptr;
				if (std::is_same<T, float>::value) {
					data = reinterpret_cast<const float*>(input[0] + processed);
				} else {
					mBuffer.set(input[0] + processed, mSize); // Type conversion
					data = mBuffer[0];
				}

				pffft_transform_ordered(mSetup, data, mRc[0], nullptr, PFFFT_FORWARD);

				// Split real and complex in two channels
				const uint sizeHalf = mSize / 2;

				// mRc[0][sizeHalf] = 0;
				// result.setFromInterleaved(mRc[0], sizeHalf, 2, processed / 2);
				result.set(mRc[0], mSize, processed);
				// result.set(mRc[0],            sizeHalf, 0, processed / 2);
				// result.set(mRc[0] + sizeHalf, sizeHalf, 1, processed / 2);

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
			uint processed = 0;
			TKLB_ASSERT(input.validSize() % mSize == 0) // Only multiples os the chunk size
			while (processed < input.validSize()) {
				const uint sizeHalf = mSize / 2;
				const uint processedHalf = processed / 2;
				mRc.set(input[0] + processedHalf, sizeHalf);
				mRc.set(input[1] + processedHalf, sizeHalf, sizeHalf);
				const T volume = 1.0 / double(mSize);
				if (std::is_same<T, float>::value) {
					float* out = reinterpret_cast<float*>(result[0]) + processed;
					pffft_transform_ordered(mSetup, mRc[0], out, nullptr, PFFFT_BACKWARD);
					result.multiply(volume); // scale the result
				} else {
					pffft_transform_ordered(mSetup, mRc[0], mBuffer[0], nullptr, PFFFT_BACKWARD);
					const float* buf = mBuffer[0];
					T* out = result[0] + processed;
					for (uint i = 0; i < mSize; i++) {
						out[i] = buf[i] * volume; // scale the result + type conversion
					}
				}
				processed += mSize;
			}
		}
	};

} // namespace

#endif // TKLB_FFT_PFFFT

