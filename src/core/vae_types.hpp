/**
 * @file vae_types.hpp
 * @author your name (you@domain.com)
 * @brief Internal types used across VAE
 * @version 0.1
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _VAE_TYPES
#define _VAE_TYPES

#include "../../external/glm/glm/glm.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../include/vae/vae.hpp"
#include <string>

namespace vae { namespace core {
	//
	template <class T> using HeapBuffer = tklb::HeapBuffer<T>;
	// How the elements are addressed
	using Size = HeapBuffer<int>::Size;
	constexpr int _VAE_SIZE_HEAPBUFFER = sizeof(HeapBuffer<int>);
	// Default sample types used whereever possible
	using Sample = float;

	using AudioBuffer = tklb::AudioBufferTpl<Sample>;
	constexpr int _VAE_SIZE_AUDIOBUFFER = sizeof(AudioBuffer);

	template <class T> using AudioBufferTpl = tklb::AudioBufferTpl<T>;

	using RingBuffer = tklb::AudioRingBufferTpl<Sample>;
	constexpr int _VAE_SIZE_RINGBUFFER = sizeof(RingBuffer);
	// Time sotred in seconds
	using Time = double;
	// Time stored in samples
	using SampleIndex = AudioBuffer::Size;

	using Uchar = unsigned char;

	using Uint = unsigned int;

	using Vec3 = glm::vec3;

	#ifdef VAE_RELEASE
		/**
		 * @brief Fake string class which does nothing
		 */
		struct NameString {
			NameString() { }
			NameString(const char*) { }
			NameString& operator= (const std::string&) { return *this; };
			NameString& operator= (std::string&&) { return *this; };
			NameString& operator= (const char*) { return *this; };
			const char* c_str() { return nullptr; }
		};
	#else
		using NameString = std::string;
	#endif // VAE_RELEASE

} } // namespace vae::core


#endif // _VAE_TYPES
