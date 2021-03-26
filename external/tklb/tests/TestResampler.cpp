#define TKLB_MAXCHANNELS 16

#include "TestCommon.hpp"
#include "../types/audio/TResampler.hpp"


int main() {
	{
		const int rateLow = 44100;
		const int rateHigh = 48000;

		const int length = 4096;
		const int blockSize = 256;
		const int channels = TKLB_MAXCHANNELS;

		Resampler<> resamplerUp(rateLow, rateHigh, blockSize);
		Resampler<> resamplerDown(rateHigh, rateLow, blockSize);

		int latency = resamplerDown.getLatency();
		latency += resamplerUp.getLatency();

		AudioBuffer bufLow, bufHigh, bufLowReference;
		bufLow.sampleRate = rateLow;
		bufHigh.sampleRate = rateHigh;

		bufLow.resize(length + 10, channels); // add some padding
		bufHigh.resize(Resampler<>::calculateBufferSize(rateLow, rateHigh, length) , channels);

		// generate sine test signal
		for (int c = 0; c < channels; c++) {
			for (int i = 0; i < length; i++) {
				bufLow[c][i] = sin(i * c * 0.001); // Fairly low frequency
			}
		}
		bufLow.setValidSize(length); // all samples in the buffer are to be processed
		bufLowReference.clone(bufLow); // Copy to compare

		resamplerUp.process(bufLow, bufHigh);
		resamplerDown.process(bufHigh, bufLow);

		// compare sine test signal
		for (int c = 0; c < channels; c++) {
			for (int i = 10; i < length - latency - 10; i++) { // crop the ends and latency
				if (!close(bufLow[c][i + latency], bufLowReference[c][i], 0.1)) {
					return 1;
				}
			}
		}
	}
	memcheck()
	return 0;
}
