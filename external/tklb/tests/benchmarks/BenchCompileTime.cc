#define TKLB_MAXCHANNELS 16
#include "../../types/audio/TAudioBuffer.hpp"
#include <algorithm>

using namespace tklb;
using namespace std;


template <class T, class T2>
void doStuff() {
	const int length = 530;
	const int channels = TKLB_MAXCHANNELS;
	AudioBufferTpl<T> bufferA[2];
	AudioBufferTpl<T2> bufferB[2];

	for (int i = 0; i < 2; i++) {
		bufferA[i].resize(length, channels);
		bufferB[i].resize(length, channels);
		for (int c = 0; c < channels; c++) {
			auto channel = bufferA[i].get(i);
			fill_n(channel, length, i);
		}
	}

	bufferA[0].add(bufferB[1]);
	bufferA[0].multiply(bufferB[1]);
	bufferA[0].set(bufferB[1]);
	bufferA[0].add(0.4);
	bufferA[0].multiply(0.4);
	bufferA[0].set(0.4);
}

int main() {
	doStuff<float, float>();
	doStuff<float, double>();
	doStuff<double, double>();
	doStuff<double, float>();
	doStuff<int, int>();
	doStuff<int, double>();
	doStuff<int, float>();
	doStuff<double, int>();
	doStuff<float, int>();

	return 0;
}
