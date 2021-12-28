#ifndef _VAE_SOURCE_LOADER
#define _VAE_SOURCE_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_source.hpp"

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
		Result load(Source& s, const char* path) {
			String joinedPath;
			const char* data = s.path.c_str();
			Size length = 0;
			if (!s.raw) {
				joinedPath = path;
				joinedPath.append(s.path);
				data = joinedPath.c_str();
			} else {
				length = s.path.size();
			}

			if (!s.stream) {
				if (s.format == Source::Format::wav) {
					VAE_PROFILER_SCOPE_NAMED("Load wav")
					bool success = tklb::wave::load<Sample, AudioBuffer>(data, s.signal, length);
					return success ? Result::Success : Result::GenericFailure;
				} else if (s.format == Source::Format::ogg) {
					VAE_PROFILER_SCOPE_NAMED("Load ogg")
					auto result = tklb::ogg::load<Sample, AudioBuffer>(data, s.signal, length);
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
