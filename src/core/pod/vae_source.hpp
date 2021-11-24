#ifndef _VAE_SOURCE
#define _VAE_SOURCE

#include "../vae_types.hpp"

#include <stddef.h>
#include <string>

namespace vae { namespace core {
	struct Source {
		enum class SourceType {
			undefined = 0,
			preload,
			stream,
			generator
		};
		enum class SourceFormat {
			undefined = 0,
			wav,
			vorbis
		};
		SourceHandle id = InvalidHandle;
		unsigned int rate;
		unsigned int channels;
		size_t length;
		SourceFormat format;
		SourceType type;
		std::string name;
		std::string path;		// Filesystem path
		AudioBuffer signal;		// Signal or scratch buffer for generated types
	};
} } // namespace vae::vore

#endif // _VAE_SOURCE
