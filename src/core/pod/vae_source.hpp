#ifndef _VAE_SOURCE
#define _VAE_SOURCE

#include "../vae_types.hpp"

#include <stddef.h>
#include <string>
#include <bitset>

namespace vae { namespace core {
	struct Source {
		struct Flags {
			enum {
				preload,	// Entire signal will be loaded into ram
				stream,		// TODO Signal will be streamed
				generator,	// TODO Signal will be generated
				wav,		// Format is wav
				vorbis,		// TODO Formatis ogg
				FLAG_COUNT
			};
		};
		SourceHandle id = InvalidSourceHandle;
		Sample gain = 1.0;		// Gain applied to every voice creatd frin this source
		AudioBuffer signal;		// Signal or scratch buffer for generated types
		std::bitset<Flags::FLAG_COUNT> flags;
		NameString name;		// Name for debugging
		PathString path;		// Filesystem path
	};

	constexpr int _VAE_STRING_SIZE = sizeof(std::string);
	constexpr int _VAE_BUF_SIZE = sizeof(AudioBuffer);
	constexpr int _VAE_SOURCE_SIZE = sizeof(Source);
} } // namespace vae::vore

#endif // _VAE_SOURCE
