#ifndef TKLB_CONVOLVER_BRUTE
#define TKLB_CONVOLVER_BRUTE

#include "../../../util/TAssert.h"
#include "../../../util/TMath.h"
#include <cmath>
#include "./../TAudioBuffer.h"

namespace tklb {
/**
 * @brief Brute force convolver using element wise multiplication.
 * Super slow especially for longer impulse responses
 */
template <typename T>
class ConvolverMonoBruteTpl {
public:
	using uchar = unsigned char;
	using uint = unsigned int;
	using Buffer = AudioBufferTpl<T>;

private:
	Buffer mIr;
	uint mIrLength;
public:

	ConvolverMonoBruteTpl() = default;

	/**
	 * @brief Load a impulse response and prepare the convolution
	 * @param buffer The ir buffer. Only the first channel is used
	 * @param channel Which channel to use from the AudioBuffer
	 * @param blockSize Size of blocks ir will be divided in
	 */
	template <typename T2>
	void load(const AudioBufferTpl<T2>& buffer, const uint blockSize, const uchar channel) {
		const T2* ir = buffer[channel];
		uint irLength = buffer.validSize();
		// trim silence, since longer IRs increase CPU usage considerably
		const T2 silence = 0.000001;
		while (irLength > 0 && silence > fabs(ir[irLength])) { irLength--; }
		if (irLength == 0) { return; }
		irLength++; // index to size
		mIr.resize(irLength, 1);
		mIr.set(ir, irLength);
		mIrLength = irLength;
	}

	/**
	 * @brief Do the convolution
	 */
	template <typename T2>
	void process(const AudioBufferTpl<T2>& in, AudioBufferTpl<T2>& out, const uchar channel) {
		const uint nf = mIrLength;
		const uint ng = in.size();
		const uint n = out.size();
		for(uint i = 0; i < n; i++) {
			const int jmn = (i >= ng - 1) ? (i - (ng - 1)) : 0;
			const int jmx = (i <  nf - 1) ?  i : (nf - 1);
			for(int j = jmn; j <= jmx; j++) {
				// nested loop goes brr
				out[channel][i] += (mIr[0][j] * in[channel][i - j]);
			}
		}
	}

};

typedef ConvolverMonoBruteTpl<float> ConvolverMonoBruteFloat;
typedef ConvolverMonoBruteTpl<double> ConvolverMonoBruteDouble;

// Default type
#ifdef TKLB_SAMPLE_FLOAT
	typedef ConvolverMonoBruteFloat ConvolverBruteMono;
#else
	typedef ConvolverMonoBruteDouble ConvolverBruteMono;
#endif


/**
 * @brief Multichannel version of the convolver
 */
template <typename T>
class ConvolverBruteTpl {
	using uchar = unsigned char;
	using uint = unsigned int;
	ConvolverMonoBruteTpl<T> mConvolvers[AudioBufferTpl<T>::MAX_CHANNELS];

public:
	using sample = T;

	ConvolverBruteTpl() = default;

	/**
	 * @brief Load a impulse response and prepare the convolution
	 * TODO make buffer const
	 */
	template <typename T2>
	void load(const AudioBufferTpl<T2>& buffer, const uint blockSize) {
		for (uchar c = 0; c < buffer.channels(); c++) {
			mConvolvers[c].load(buffer, blockSize, c);
		}
	}

	/**
	 * @brief Load a impulse response and prepare the convolution
	 */
	template <typename T2>
	void process(const AudioBufferTpl<T2>& inBuf, AudioBufferTpl<T2>& outBuf) {
		for (uchar c = 0; c < inBuf.channels(); c++) {
			mConvolvers[c].process(inBuf, outBuf, c);
		}
	}
};

typedef ConvolverBruteTpl<float> ConvolverBruteFloat;
typedef ConvolverBruteTpl<double> ConvolverBruteDouble;

// Default type
#ifdef TKLB_SAMPLE_FLOAT
	typedef ConvolverBruteFloat ConvolverBrute;
#else
	typedef ConvolverBruteDouble ConvolverBrute;
#endif

} // namespace

#endif // TKLB_CONVOLVER_BRUTE
