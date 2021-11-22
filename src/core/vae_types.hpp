#ifndef _VAE_TYPES
#define _VAE_TYPES

#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../include/vae/vae.hpp"

namespace vae { namespace core {
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
} } // namespace vae::core


#endif // _VAE_TYPES
