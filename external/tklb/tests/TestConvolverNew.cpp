#define TKLB_LEAKCHECKER_DISARM
#define TKLB_USE_OOURA
#include "TestCommon.hpp"
// #define TKLB_NO_SIMD
#include "../types/audio/convolver/TConvolverNew.hpp"


int main() {
	{
		const int audioLength = 48000 * 10;
		const int audioChannels = 2;
		const int blockSize = 128;

		using T = Convolver::sample;
		Convolver con;
		AudioBuffer ir, in, out;
		ir.resize(1024, audioChannels);
		ir.set(0);
		for (int c = 0; c < audioChannels; c++) {
			ir[c][1] = 1.0; // perfect impulse delaying the signal by one sample
		}
		con.load(ir, blockSize);

		in.resize(audioLength, audioChannels);
		out.resize(in);

		for(int c = 0; c < audioChannels; c++) {
			for(int i = 0; i < audioLength; i++) {
				in[c][i] = (i + c) % 2;
			}
		}

		con.process(in, out);

		/**
		 * Skip the first and last sample due to the delay
		 */
		for (char c = 0; c < audioChannels; c++) {
			for(int i = 1; i < (audioLength - 1); i++) {
				if (!close(in[c][i], out[c][i + 1])) {
					return 1;
				}
			}
		}
	}

	memcheck()
	return 0;
}
