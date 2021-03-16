#define TKLB_LEAKCHECKER_DISARM
#define TKLB_MAXCHANNELS 16
#include "BenchmarkCommon.h"
#include "../../types/audio/TAudioBuffer.h"
#include "../../types/audio/convolver/TConvolverOld.h"


int main() {
	{
		Convolver con;
		const int audioLength = 520;
		int blockSize = 128;

		AudioBufferFloat ir, in, out;
		ir.resize(1024, TKLB_MAXCHANNELS);
		ir.set(0);
		for (int c = 0; c < TKLB_MAXCHANNELS; c++) {
			ir[c][1] = 1.0; // perfect impulse delaying the signal by one sample
			ir[c][512] = 1.0; // second delay to make the ir longer
		}
		con.load(ir, blockSize);

		in.resize(audioLength, TKLB_MAXCHANNELS);
		out.resize(in);

		for(int c = 0; c < TKLB_MAXCHANNELS; c++) {
			for(int i = 0; i < audioLength; i++) {
				in[c][i] = (i + c) % 2;
			}
		}

		{
			TIMER(Microseconds);
			for(int i = 0; i < ITERATIONS; i++) {
				con.process(in, out);
			}
		}

	}
	return 0;
}
