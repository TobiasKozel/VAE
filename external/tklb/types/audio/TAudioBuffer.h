#ifndef TKLB_AUDIOBUFFER
#define TKLB_AUDIOBUFFER

#include <cstring>
#include <algorithm>

#ifndef TKLB_NO_SIMD
	#include "../../external/xsimd/include/xsimd/xsimd.hpp"
#endif

#include "../THeapBuffer.h"
#include "../../util/TAssert.h"

#ifdef TKLB_MAXCHANNELS
	#if TKLB_MAXCHANNELS == 1
		#error "Setting TKLB_MAXCHANNELS lower than 2 will break FFTs and Convolution."
	#endif

	#if TKLB_MAXCHANNELS == 0
		#error "Setting TKLB_MAXCHANNELS to 0 is not an option."
	#endif
#endif

namespace tklb {

/**
 * @brief Class for handling the most basic audio needs
 * Does convenient type conversions
 */
template <typename T>
class AudioBufferTpl {

public:
	using uchar = unsigned char;
	using uint = unsigned int;
#ifdef TKLB_MAXCHANNELS
	static constexpr uchar MAX_CHANNELS = TKLB_MAXCHANNELS;
#else
	static constexpr uchar MAX_CHANNELS = 2;
#endif

	/**
	 * @brief Sample type exposed for convenience
	 */
	using sample = T;

	template <class T2>
	/**
	 * @brief Aligned vector type
	 */
	using Buffer = HeapBuffer<T2
	#ifndef TKLB_NO_SIMD
		, xsimd::aligned_allocator<T2, XSIMD_DEFAULT_ALIGNMENT>
	#endif
	>;

#ifndef TKLB_NO_SIMD
	static constexpr uint stride = xsimd::simd_type<T>::size;
#endif

private:
	Buffer<T> mBuffers[MAX_CHANNELS];
	T* mRawBuffers[MAX_CHANNELS];
	uchar mChannels = 0;
	uint mSize = 0;
	uint mValidSize = 0;

public:
	/**
	 * @brief Only relevant for resampling and oversampling.
	 */
	uint sampleRate = 0;

	/**
	 * @brief Empty buffer with no memory allocated yet
	 */
	AudioBufferTpl() { }

	/**
	 * @brief Buffer with memory allocated
	 */
	AudioBufferTpl(const uint length, const uchar channels) {
		resize(length, channels);
	}

	/**
	 * @brief Set a single channel from an array
	 * @param samples An Array containing the audio samples
	 * @param length Amount of sample to copy
	 * @param channel Channel index
	 * @param offset The offset for the traget buffer
	 */
	template <typename T2>
	void set(
		const T2* samples,
		uint length,
		const uchar channel = 0,
		const uint offset = 0
	) {
		if (mChannels <= channel) { return; }
		TKLB_ASSERT(size() >= offset)
		length = std::min(length, size() - offset);
		T* out = get(channel);
		if (std::is_same<T2, T>::value) {
			memcpy(out + offset, samples, sizeof(T) * length);
		} else {
			for (uint i = 0; i < length; i++) {
				out[i + offset] = static_cast<T>(samples[i]);
			}
		}
	}

	/**
	 * @brief Set multiple channels from a 2D array
	 * @param samples A 2D Array containing the audio samples (float or double)
	 * @param length Samples to copy in (single channel)
	 * @param channels Channel count
	 * @param offset Offset in the target buffer
	 * @param offsetIn Offset in the source buffer
	 */
	template <typename T2>
	void set(
		T2** const samples,
		const uint length,
		const uchar channels,
		const uint offset = 0,
		const uint offsetIn = 0
	) {
		for (uchar c = 0; c < channels; c++) {
			set(samples[c] + offsetIn, length, c, offset);
		}
	};

	/**
	 * @brief Set from another buffer object
	 * e.g. offset=10 and length=15 will copy 15 samples into the buffer starting at the 10th sample
	 * @param buffer Source buffer object
	 * @param length Samples to copy in
	 * @param offset Start offset in the target buffer
	 * @param offsetIn Start offset in the source buffer
	 */
	template <typename T2>
	void set(
		const AudioBufferTpl<T2>& buffer,
		uint length = 0,
		const uint offset = 0,
		const uint offsetIn = 0
	) {
		length = length == 0 ? buffer.size() : length;
		for (uchar c = 0; c < buffer.channels(); c++) {
			set(buffer[c] + offsetIn, length, c, offset);
		}
	};

	/**
	 * @brief Set the entire buffer to a constant value
	 * @param value Value to fill the buffer with
	 * @param length Samples to set. 0 Sets all
	 * @param offset Start offset in the target buffer
	 */
	void set(
		T value = 0,
		uint length = 0,
		const uint offset = 0
	) {
		TKLB_ASSERT(size() >= offset)
		length = std::min(size() - offset, length ? length : size());
		for (uchar c = 0; c < channels(); c++) {
			fill_n(get(c) + offset, length, value);
		}
	}

	/**
	 * @brief Set multiple channels from an interleaved array
	 * @param samples A 1D Array containing the interleaved audio samples (float or double)
	 * @param length The length of a single channel
	 * @param channels Channel count
	 * @param offset Start offset in the target buffer
	 * @param offsetIn Start offset in the source buffer
	 */
	template <typename T2>
	void setFromInterleaved(
		const T2* samples,
		uint length,
		const uchar channels,
		const uint offset = 0,
		uint offsetIn = 0
	) {
		TKLB_ASSERT(size() >= offset)
		length = std::min(size() - offset, length);
		offsetIn *= channels;
		for (uchar c = 0; c < std::min(channels, mChannels); c++) {
			T* out = get(c);
			for(uint i = 0, j = c + offsetIn; i < length; i++, j+= channels) {
				out[i + offset] = static_cast<T>(samples[j]);
			}
		}
	}

	/**
	 * @brief Resizes the buffer to the desired length and channel count.
	 * @param length The desired length in Samples. 0 will deallocate.
	 * @param channels Desired channel count. 0 will deallocate.
	 */
	void resize(const uint length, uchar channels) {
		if (channels == mChannels && size() == length) {
			return;
		}
		TKLB_ASSERT(channels <= MAX_CHANNELS)
		channels = std::min(channels, uchar(MAX_CHANNELS));
		for(uchar c = 0; c < channels; c++) {
			mBuffers[c].resize(length);
		}
		for (uchar c = channels; c < MAX_CHANNELS; c++) {
			mBuffers[c].resize(0);
		}
		mChannels = channels;
		mSize = length;
		mValidSize = std::min(mValidSize, mSize);
	}

	/**
	 * @brief Resize to the provided length and keep the channelcount.
	 * If the channel count was 0 it will assume 1 channel instead.
	 * @param length The desired length in samples. 0 will deallocate.
	 */
	void resize(const uint length) {
		resize(length, std::max(uchar(1), mChannels));
	}

	/**
	 * @brief Resize to match the provided buffer
	 */
	template <typename T2>
	void resize(const AudioBufferTpl<T2>& buffer) {
		resize(buffer.size(), buffer.channels());
	}

	/**
	 * @brief Add the provided buffer
	 * @param buffer Source buffer object
	 * @param length Samples to add from the source buffer
	 * @param offset Start offset in the target buffer
	 * @param offsetIn Start offset in the source buffer
	 */
	template <typename T2>
	void add(
		const AudioBufferTpl<T2>& buffer,
		uint length = 0,
		uint offset = 0,
		uint offsetIn = 0
	) {
		TKLB_ASSERT(size() >= offset)
		TKLB_ASSERT(buffer.size() >= offsetIn)
		length = length == 0 ? buffer.size() - offsetIn : length;
		length = std::min(buffer.size() - offsetIn, size() - offset);
		const uchar channelCount = std::min(buffer.channels(), channels());

		#ifndef TKLB_NO_SIMD
			if (std::is_same<T2, T>::value) {
				const uint vectorize = length - (length % stride);
				for (uchar c = 0; c < channelCount; c++) {
					T* out = get(c) + offset;
					const T* in = reinterpret_cast<const T*>(buffer[c]) + offsetIn;
					for(uint i = 0; i < vectorize; i += stride) {
						xsimd::simd_type<T> a = xsimd::load_aligned(in + i);
						xsimd::simd_type<T> b = xsimd::load_aligned(out + i);
						xsimd::store_aligned(out + i, (a + b));
					}
					for(uint i = vectorize; i < length; i++) {
						out[i] += in[i];
					}
				}
				return;
			}
		#endif

		// If the type doen't match or simd is diabled
		for (uchar c = 0; c < channelCount; c++) {
			T* out = get(c) + offset;
			const T2* in = buffer[c] + offsetIn;
			for(uint i = 0; i < length; i++) {
				out[i] += in[i];
			}
		}
	}

	/**
	 * @brief Multiply two buffers
	 * @param length Samples to multiply from the source buffer
	 * @param offset Start offset in the target buffer
	 * @param offsetIn Start offset in the source buffer
	 */
	template <typename T2>
	void multiply(
		const AudioBufferTpl<T2>& buffer,
		uint length = 0,
		uint offset = 0,
		uint offsetIn = 0
	) {
		TKLB_ASSERT(size() >= offset)
		TKLB_ASSERT(buffer.size() >= offsetIn)
		length = length == 0 ? buffer.size() - offsetIn : length;
		length = std::min(buffer.size() - offsetIn, size() - offset);
		const uchar channelsCount = std::min(buffer.channels(), channels());

		#ifndef TKLB_NO_SIMD
			if (std::is_same<T2, T>::value) {
				const uint vectorize = length - (length % stride);
				for (uchar c = 0; c < channelsCount; c++) {
					T* out = get(c) + offset;
					const T* in = reinterpret_cast<const T*>(buffer[c]) + offsetIn;
					for(uint i = 0; i < vectorize; i += stride) {
						xsimd::simd_type<T> a = xsimd::load_aligned(in + i);
						xsimd::simd_type<T> b = xsimd::load_aligned(out + i);
						xsimd::store_aligned(out + i, (a * b));
					}
					for(uint i = vectorize; i < length; i++) {
						out[i] *= in[i];
					}
				}
				return;
			}
		#endif

		// If the type doen't match or simd is diabled
		for (uchar c = 0; c < channelsCount; c++) {
			T* out = get(c) + offset;
			const T2* in = buffer[c] + offsetIn;
			for(uint i = 0; i < length; i++) {
				out[i] *= in[i];
			}
		}
	}

	/**
	 * @brief Mutliplies the content of the buffer with a constant
	 * @param value Constant to multiply the buffer with
	 */
	void multiply(T value) {
		const uint length = size();

		#ifndef TKLB_NO_SIMD
			const uint vectorize = length - (length % stride);
			for (uchar c = 0; c < channels(); c++) {
				T* out = get(c);
				for(uint i = 0; i < vectorize; i += stride) {
					xsimd::simd_type<T> b = xsimd::load_aligned(out + i);
					xsimd::store_aligned(out + i, (b * value));
				}
				for(uint i = vectorize; i < length; i++) {
					out[i] *= value;
				}
			}
		#else
			for (uchar c = 0; c < channels(); c++) {
				T* out = get(c);
				for(uint i = 0; i < length; i++) {
					out[i] *= value;
				}
			}
		#endif
	}

	/**
	 * @brief Adds a constant to the contents of the buffer
	 * @param value
	 */
	void add(T value) {
		const uint length = size();

		#ifndef TKLB_NO_SIMD
			const uint vectorize = length - (length % stride);
			for (uchar c = 0; c < channels(); c++) {
				T* out = get(c);
				for(uint i = 0; i < vectorize; i += stride) {
					xsimd::simd_type<T> b = xsimd::load_aligned(out + i);
					xsimd::store_aligned(out + i, (b + value));
				}
				for(uint i = vectorize; i < length; i++) {
					out[i] += value;
				}
			}
		#else
			for (uchar c = 0; c < channels(); c++) {
				T* out = get(c);
				for(uint i = 0; i < length; i++) {
					out[i] += value;
				}
			}
		#endif
	}

	/**
	 * @brief Inject forgeign memory to be used by the buffer.
	 * Potentially dangerous but useful when splitting up channels for processing
	 * @param mem Modifiable memory (No type conversions here)
	 * @param size Size of the buffer
	 * @param channel The channel index
	 */
	void inject(T* mem, const uint size, const uchar channel = 0) {
		mBuffers[channel].inject(mem, size);
		mSize = size;
		mValidSize = size;
	}

	/**
	 * @brief Inject const forgeign memory to be used by the buffer.
	 * Potentially dangerous but useful when splitting up channels for processing
	 * @param mem Const memory (No type conversions here)
	 * @param size Size of the buffer
	 * @param channel The channel index
	 */
	void inject(const T* mem, const uint size, const uchar channel = 0) {
		mBuffers[channel].inject(mem, size);
		mSize = size;
		mValidSize = size;
	}

	/**
	 * @brief Returns the amount of channels
	 */
	uchar channels() const { return mChannels; }

	/**
	 * @brief Returns the allocated length of the buffer
	 */
	uint size() const { return mSize; }

	/**
	 * @brief fReturns the length of actually valid audio in the buffer
	 */
	uint validSize() const {
		return mValidSize ? mValidSize : size();
	}

	/**
	 * @brief Set the amount of valid samples currently in the buffer
	 * This is mostly a convenience flag since the actual size of the buffer may be larger
	 */
	void setValidSize(const uint v) {
		TKLB_ASSERT(size() >= v);
		mValidSize = v;
	}

	T* get(const uchar channel) {
		TKLB_ASSERT(channel < mChannels)
		return mBuffers[channel].data();
	};

	const T* get(const uchar channel) const {
		TKLB_ASSERT(channel < mChannels)
		return mBuffers[channel].data();
	};

	const T* operator[](const uchar channel) const { return get(channel); }

	T* operator[](const uchar channel) { return get(channel); }

	/**
	 * @brief Returns a array owned by the object containing pointers to all the channels.
	 * <b>Don't</b> call this constantly, try to cache the returned value for reuse
	 */
	T** getRaw() {
		for (uchar c = 0; c < mChannels; c++) {
			mRawBuffers[c] = get(c);
		}
		return mRawBuffers;
	}

	/**
	 * @brief Fill the provided array with the contents of this buffer
	 * @param target The arry to fill
	 * @param channel Which source channel to use
	 * @param length The length of the output
	 * @param offset Start offset in the source (this) buffer
	 * @return The amount of samples written, might be less than requested
	 */
	template <typename T2>
	uint put(
		T2* target,
		uint length = 0,
		const uchar channel = 0,
		const uint offset = 0
	) const {
		if (mChannels <= channel) { return 0; }
		TKLB_ASSERT(size() >= offset)
		length = length == 0 ? size() : length;
		length = std::min(length, size() - offset);
		const T2* source = get(channel) + offset;
		if (std::is_same<T2, T>::value) {
			memcpy(target, source, sizeof(T) * length);
		} else {
			for (uint i = 0; i < length; i++) {
				target[i] = T2(source[i]);
			}
		}
		return length;
	}

	/**
	 * @brief Fill the provided 2D array with the contents of this buffer
	 * @param target The array to fill
	 * @param channels How many channels there are in the target buffer
	 * @param length The length of the output
	 * @return The amount of samples written, might be less than requested
	 */
	template <typename T2>
	uint put(T2** target,
		const uint length = 0,
		uchar channels = 0,
		const uint offset = 0
	) const {
		uint res = 0;
		channels = (channels == 0) ? mChannels : channels;
		for (uchar c = 0; c < channels; c++) {
			res = put(target[c], length, c, offset);
		}
		return res;
	}

	/**
	 * @brief Puts the interleaved contents in the target buffer
	 * @param buffer The array to fill with interleaved audio
	 * @param length Frames to interleave (not the length of the inerleaved result)
	 * @param offset Offset for the sourcebuffer (this)
	 * @return The total length of the interleaved result
	 */
	template <typename T2>
	uint putInterleaved(
		T2* buffer,
		uint length = 0,
		const uint offset = 0
	) const {
		TKLB_ASSERT(size() >= offset)
		const uchar chan = channels();
		length = (length == 0) ? size() : length;
		length = std::min(size() - offset, length);
		uint out = 0;
		for (int i = 0; i < length; i++) {
			for (uchar c = 0; c < chan; c++) {
				buffer[out] = mBuffers[c][i];
				out++;
			}
		}
	}
};


typedef AudioBufferTpl<float> AudioBufferFloat;
typedef AudioBufferTpl<double> AudioBufferDouble;

// Default type
#ifdef TKLB_SAMPLE_FLOAT
	typedef AudioBufferFloat AudioBuffer;
#else
	typedef AudioBufferDouble AudioBuffer;
#endif

}

#endif // TKLB_AUDIOBUFFER
