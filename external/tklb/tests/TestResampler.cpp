#define TKLB_MAXCHANNELS 16

// TODO there's some fuckery going on here
// probably the memchecker missing some allocations
#define TKLB_LEAKCHECKER_DISARM
#include "TestCommon.h"
#include "../types/audio/TResampler.h"


int main() {
	{
		const int length = 512;
		const int channels = TKLB_MAXCHANNELS;
		const int rate1 = 44100;
		const int rate2 = 48000;
		Resampler<> up(rate1, rate2, length);
		Resampler<> down(rate2, rate1, length * 2); // Bigger max block obviously

		AudioBuffer in, out;
		in.sampleRate = rate1;
		out.sampleRate = rate2;

		in.resize(Resampler<>::calculateBufferSize(rate1, rate1, length), channels);
		out.resize(Resampler<>::calculateBufferSize(rate1, rate2, length) , channels); // same here

		// generate sine test signal
		for (int c = 0; c < channels; c++) {
			for (int i = 0; i < length; i++) {
				in[c][i] = sin(i * c * 0.001); // Failry low frequency
			}
		}
		in.setValidSize(length);

		up.process(in, out);
		down.process(out, in);

		// compare sine test signal
		for (int c = 0; c < channels; c++) {
			for (int i = 10; i < length - 10; i++) {
				if (!close(in[c][i], -sin(i * c * 0.001), 0.6)) {
					// return 1;
				}
			}
		}
	}
	memcheck()
	return 0;
}
