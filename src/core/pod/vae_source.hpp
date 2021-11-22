#ifndef _VAE_SOURCE
#define _VAE_SOURCE

#include "../vae_types.hpp"

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
		SourceHandle id;
		std::string name;
		std::string path;
		SourceFormat format;
		SourceType type;
		size_t length;
		unsigned int rate;
		unsigned int channels;
	};
} } // namespace vae::vore

#endif // _VAE_SOURCE
