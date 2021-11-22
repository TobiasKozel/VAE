#ifndef _VAE_TYPES
#define _VAE_TYPES

#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"

namespace vae {
	/**
	 * Types used internally
	 */
	namespace core {
		//
		template <class T> using HeapBuffer = tklb::HeapBuffer<T>;
		// How the elements are addressed
		using Size = HeapBuffer<int>::Size;
		// Default sample types used whereever possible
		using Sample = float;
		using AudioBuffer = tklb::AudioBufferTpl<Sample>;
		using RingBuffer = tklb::AudioRingBufferTpl<Sample>;
		// Time sotred in seconds
		using Time = double;
		// Time stored in samples
		using SampleIndex = AudioBuffer::Size;
	}
}


#endif // _VAE_TYPES
