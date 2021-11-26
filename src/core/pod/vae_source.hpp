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
		SourceFormat format;
		SourceType type;
		std::string name;
		std::string path;		// Filesystem path
		AudioBuffer signal;		// Signal or scratch buffer for generated types
	};

	constexpr int _VAE_STRING_SIZE = sizeof(std::string);
	constexpr int _VAE_BUF_SIZE = sizeof(AudioBuffer);
	constexpr int _VAE_SOURCE_SIZE = sizeof(Source);
} } // namespace vae::vore

#endif // _VAE_SOURCE
