#ifndef _VAE_SOURCE_LOADER
#define _VAE_SOURCE_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_source.hpp"

#include <string>
#include "../../../external/tklb/src/types/audio/TWaveFile.hpp"
#include "../../../external/tklb/src/types/audio/TOggFile.hpp"


namespace vae { namespace core {
	struct SourceLoader {
		/**
		 * @brief Loads a wav file for the resource
		 * @param s
		 * @param path
		 * @return Result
		 */
		Result load(Source& s, std::string path) {
			path = path + s.path;
			if (!s.stream) {
				if (s.format == Source::Format::wav) {
					VAE_PROFILER_SCOPE_NAMED("Load wav")
					auto result = tklb::wave::load(path.c_str(), s.signal);
					return result ? Result::Success : Result::GenericFailure;
				} else if (s.format == Source::Format::ogg) {
					VAE_PROFILER_SCOPE_NAMED("Load ogg")
					auto result = tklb::ogg::load(path.c_str(), s.signal);
					return result ? Result::Success : Result::GenericFailure;
				}
				return Result::GenericFailure; // what format is this?
			}
			// TODO STREAM
			return Result::GenericFailure;
		}
	};
} } // vae::core


#endif // _VAE_SOURCE_LOADER
