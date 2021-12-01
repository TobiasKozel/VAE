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
#include <vector>

namespace vae { namespace core {
	//
	template <class T> using HeapBuffer = std::vector<T>;
	// template <class T> using HeapBuffer = tklb::HeapBuffer<T>;


	constexpr int _VAE_SIZE_TKLB = sizeof(tklb::HeapBuffer<int>);
	constexpr int _VAE_SIZE_STD = sizeof(std::vector<int>);

	using Sample = float;					// Default sample types used whereever possible
	using Size = unsigned int;				// How the elements are addressed in the heapbuffer
	using Time = double;					// Time sotred in seconds
	using Uchar = unsigned char;
	using Uint = unsigned int;
	using Vec3 = glm::vec3;

	using AudioBuffer = tklb::AudioBufferTpl<Sample>;
	template <class T> using AudioBufferTpl = tklb::AudioBufferTpl<T>;
	using SampleIndex = AudioBuffer::Size;	// Time stored in samples
	constexpr int _VAE_SIZE_AUDIOBUFFER = sizeof(AudioBuffer);

	using RingBuffer = tklb::AudioRingBufferTpl<Sample>;
	constexpr int _VAE_SIZE_RINGBUFFER = sizeof(RingBuffer);

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
		using NameString = std::string;	// Used for debug names
	#endif // VAE_RELEASE

		using PathString = std::string;	// Used for path strings

} } // namespace vae::core


#endif // _VAE_TYPES
