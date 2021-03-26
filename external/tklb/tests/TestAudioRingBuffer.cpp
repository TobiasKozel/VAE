#include "./TestCommon.hpp"

#include "../types/audio/TAudioBuffer.hpp"
#include "../types/audio/TAudioRingBuffer.hpp"
int main() {
	{
		const int size = 1024;
		const int sourceSize = size * 10;
		const int channels = 4;
		AudioBuffer source, dest;
		source.resize(sourceSize, channels);
		dest.resize(sourceSize, channels);

		// generate sine test signal
		for (int c = 0; c < channels; c++) {
			for (int i = 0; i < sourceSize; i++) {
				source[c][i] = sin(i * (c + 2) * 0.001);
			}
		}

		{
			AudioRingBuffer buffer(size, channels);
			int inpos = 0;
			int outpos = 0;
			inpos += buffer.push(source, 100, inpos);
			inpos += buffer.push(source, 200, inpos);

			outpos += buffer.pop(dest, 50, 0, outpos);

			inpos += buffer.push(source, 100, inpos);

			outpos += buffer.pop(dest, 350, 0, outpos);

			if (buffer.filled() != 0) {
				return 1;
			}

			if (buffer.free() != size) {
				return 2;
			}

		}

		// compare sine test signal
		for (int c = 0; c < channels; c++) {
			for (int i = 0; i < 400; i++) {
				if (!close(source[c][i], dest[c][i], 0.1)) {
					return 3;
				}
			}
		}
	}

	memcheck()
	return 0;
}
