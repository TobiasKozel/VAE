#define TKLB_MAXCHANNELS 16
#include "../../types/audio/TAudioBuffer.h"

#define ITERATIONS 10000
#include "BenchmarkCommon.h"

int main() {
	// some overhang so simd can't do all of it
	const int length = 530;
	const int channels = TKLB_MAXCHANNELS;
	AudioBuffer buffers[2];

	for (int i = 0; i < 2; i++) {
		buffers[i].resize(length, channels);
		for (int c = 0; c < channels; c++) {
			auto channel = buffers[i].get(i);
			fill_n(channel, length, i);
		}
	}

	{
		TIMER(Microseconds);
		for(int i = 0; i < ITERATIONS; i++) {
			buffers[0].add(buffers[1]);
		}
	}
	return 0;
}
