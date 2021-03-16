#ifndef TKLB_CONVOLVER
#define TKLB_CONVOLVER

#include "../../../util/TNoCopy.h"
#include "../../../util/TAssert.h"
#include "../TAudioBuffer.h"

#define TKLB_CONVOLUTION_FLOAT

/**
 * Decide which type the convolution will use
 */
#ifndef TKLB_CONVOLUTION_FLOAT
	#define FFTCONVOLVER_TYPE double
	#ifdef TKLB_SAMPLE_FLOAT
		#define TKLB_CONVOLUTION_NEEDS_CONVERSION
	#endif
#else
#define FFTCONVOLVER_TYPE float
	// float convolution can use SSE
	#ifdef TKLB_NO_SIMD
		#define FFTCONVOLVER_DONT_USE_SSE
	#else
		#define FFTCONVOLVER_USE_SSE
	#endif
	#ifndef TKLB_SAMPLE_FLOAT
		#define TKLB_CONVOLUTION_NEEDS_CONVERSION
	#endif
#endif

#include "../../../external/convolver/twoStageConvolver.h"

#include <atomic>

namespace tklb {
/**
 * Wraps up the FttConvolver to support type conversion
 */
class ConvolverMono {

public:
	using uchar = unsigned char;
	using uint = unsigned int;

private:
	fftconvolver::FFTConvolver mConvolver;
	AudioBufferTpl<fftconvolver::Sample> mConversion;
	uint mBlockSize;

public:
	ConvolverMono() = default;
	/**
	 * @brief Load a impulse response and prepare the convolution
	 * @param buffer The ir buffer. Only the first channel is used
	 * @param channel Which channel to use from the AudioBuffer
	 * @param blockSize Size of blocks ir will be divided in
	 */
	template <typename T2>
	void load(const AudioBufferTpl<T2>& buffer, const uint blockSize, const uchar channel) {
		const fftconvolver::Sample* ir = nullptr;
		const uint irLength = buffer.validSize();
		if (std::is_same<T2, fftconvolver::Sample>::value) {
			ir = reinterpret_cast<const fftconvolver::Sample*>(buffer[channel]);
		} else {
			mConversion.resize(irLength, 2);
			mConversion.set(buffer[channel], irLength);
			ir = mConversion[0];
		}
		mConversion.resize(blockSize); // incase we need to convert
		mConvolver.init(blockSize, ir, irLength);
		mBlockSize = blockSize;
	}

	/**
	 * @brief Do the convolution
	 */
	template <typename T2>
	void process(const AudioBufferTpl<T2>& inBuf, AudioBufferTpl<T2>& outBuf, const uchar channel) {
		const uint length = inBuf.validSize();
		uint samplesLeft = length;
		for (uint i = 0; i < length; i += mBlockSize) {
			const uint remaining = std::min(mBlockSize, samplesLeft);
			const fftconvolver::Sample* in = nullptr;
			fftconvolver::Sample* out = nullptr;
			if (std::is_same<T2, fftconvolver::Sample>::value) {
				in = reinterpret_cast<const fftconvolver::Sample*>(inBuf[channel] + i);
				out = reinterpret_cast<fftconvolver::Sample*>(outBuf[channel] + i);
			} else {
				mConversion.set(inBuf[channel]  + i, remaining, 0);
				mConversion.set(outBuf[channel] + i, remaining, 1);
				in = mConversion[0];
				out = mConversion[1];
			}
			mConvolver.process(in, out, remaining);
			samplesLeft -= remaining;
		}
	}

	static const char* getLicense() {
		return
			"Realtime Convolution by\n"
			"https://github.com/HiFi-LoFi\n"
			"https://github.com/HiFi-LoFi/FFTConvolver\n"
			"MIT License\n\n";
	}
};


/**
 * @brief Multichannel version of the convolver
 */
class Convolver {
	using uchar = unsigned char;
	using uint = unsigned int;
	ConvolverMono mConvolvers[AudioBuffer::MAX_CHANNELS];

public:
	using sample = fftconvolver::Sample;

	Convolver() = default;

	/**
	 * @brief Load a impulse response and prepare the convolution
	 */
	template <typename T2>
	void load(const AudioBufferTpl<T2>& buffer, const uint blockSize) {
		const uint size = buffer.validSize();
		for (uchar c = 0; c < buffer.channels(); c++) {
			mConvolvers[c].load(buffer, blockSize, c);
		}
	}

	/**
	 * @brief Load a impulse response and prepare the convolution
	 */
	template <typename T2>
	void process(const AudioBufferTpl<T2>& inBuf, AudioBufferTpl<T2>& outBuf) {
		const uint size = inBuf.validSize();
		for (uchar c = 0; c < inBuf.channels(); c++) {
			mConvolvers[c].process(inBuf, outBuf, c);
		}
	}
};

} // namespace

#endif // TKLB_CONVOLVER
