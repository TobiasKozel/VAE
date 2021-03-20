#include "./TestCommon.hpp"
#include "../types/audio/TAudioBuffer.hpp"


const int length = 1024;
const int channels = 2;

int deinterleave(AudioBuffer& buffer) {
	buffer.resize(length, channels);

	float noconvInterleaved[length * channels];

	for (int i = 0; i < length * channels; i += channels) {
		noconvInterleaved[i] = 1.0;
		noconvInterleaved[i + 1] = 0.0;
	}

	buffer.setFromInterleaved(noconvInterleaved, length, channels);

	auto deinterleavedL = buffer[0];
	auto deinterleavedR = buffer[1];

	for (int i = 0; i < length; i++) {
		if (!close(deinterleavedL[i], 1.0)) {
			return 1;
		}
		if (!close(deinterleavedR[i], 0.0)) {
			return 2;
		}
	}

	if (buffer.size() != length) {
		return 3;
	}

	if (buffer.channels() != channels) {
		return 4;
	}
	return 0;
}

template <typename T>
int conversion(AudioBuffer& buffer) {
	buffer.resize(0);
	buffer.resize(length, channels);
	buffer.set(0); // important
	T fsamples[channels * length];
	std::fill_n(fsamples, channels * length, 1.0); // only set part of it to one
	T* fbuf[channels] = { };

	for (int c = 0; c < channels; c++) {
		fbuf[c] = fsamples + (length * c);
	}

	buffer.set(fbuf, length, channels, length / 2);

	AudioBuffer::sample* l = buffer[0];
	AudioBuffer::sample* r = buffer[1];

	for (int i = 0; i < length; i++) {
		AudioBuffer::sample expected = (i >= (length / 2)) ? 1.0 : 0;
		if (!close(expected, l[i])) {
			return 5;
		}
		if (!close(expected, r[i])) {
			return 6;
		}
	}

	return 0;
}

int add() {
	AudioBuffer buffer, buffer2;
	buffer.resize(length, channels);
	buffer2.resize(buffer);

	AudioBuffer::sample fsamples[channels * length];
	AudioBuffer::sample* fbuf[channels] = { };

	/**
	 * Set the last half to 1.0 of the first buffer
	 */
	fill_n(fsamples, channels * length, 1.0);
	for (int c = 0; c < channels; c++) {
		fbuf[c] = fsamples + (length * c);
	}
	buffer.set(fbuf, length, channels, length / 2);

	/**
	 * Fill the first half with 1.0 of the second buffer
	 */
	fill_n(fsamples, channels * length, 0.0);
	for (int c = 0; c < channels; c++) {
		for (int i = 0; i < (length / 2); i++) {
			fbuf[c][i] = 1.0;
		}
	}
	buffer2.set(fbuf, length, channels);

	/**
	 * Add them together
	 */
	buffer.add(buffer2);

	auto out = buffer.get(0);

	/**
	 * Check if the whole buffer is 1.0 now
	 */
	for (int i = 0; i < length; i++) {
		if (!close(out[i], 1.0)) {
			return 7;
		}
	}
	return 0;
}

int casts() {
	AudioBufferDouble d1, d2;
	AudioBufferFloat f1, f2;
	d1.resize(length, channels);
	d2.resize(d1);
	f1.resize(d1);
	f2.resize(d1);

	d1.add(d2);
	d1.set(d2);
	d1.multiply(d2);
	d2.add(f2);
	d2.set(f2);
	d2.multiply(f2);

	f1.add(f2);
	f1.set(f2);
	f1.multiply(f2);
	f1.add(d2);
	f1.set(d2);
	f1.multiply(d2);
	return 0;
}

int main() {


	{
		AudioBuffer buffer;

		returnNonZero(deinterleave(buffer))
		returnNonZero(conversion<float>(buffer))
		returnNonZero(conversion<double>(buffer))

		returnNonZero(add())

		returnNonZero(casts())
	}

	memcheck()
	return 0;
}
