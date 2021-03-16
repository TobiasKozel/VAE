#define TKLB_MAXCHANNELS 16
#include "../../types/audio/TAudioBuffer.h"

#define ITERATIONS 10000
#include "BenchmarkCommon.h"

int main() {
	// some overhang so simd can't do all of it
	const int length = 530;
	const int channels = TKLB_MAXCHANNELS;
	AudioBuffer buffer;

	buffer.resize(length, channels);
	for (int c = 0; c < channels; c++) {
		auto channel = buffer.get(c);
		fill_n(channel, length, 2);
	}

	{
		TIMER(Microseconds);
		for(int i = 0; i < ITERATIONS; i++) {
			buffer.add(10.0);
		}
	}
	return 0;
}
