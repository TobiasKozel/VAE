#ifndef _VAE_TYPES
#define _VAE_TYPES

#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"

namespace vae { namespace types {
	using Sample = float;
	using Size = tklb::HeapBuffer<int>::Size;
	using AudioBuffer = tklb::AudioBufferTpl<Sample>;
	using RingBuffer = tklb::AudioRingBufferTpl<Sample>;
	using Time = double;
	using SampleIndex = AudioBuffer::Size;
} }


#endif // _VAE_TYPES
