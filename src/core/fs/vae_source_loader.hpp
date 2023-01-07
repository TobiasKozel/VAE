#ifndef _VAE_SOURCE_LOADER
#define _VAE_SOURCE_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_source.hpp"

#ifndef VAE_NO_WAV
	#include "../../../external/tklb/src/types/audio/TWave.hpp"
#endif
#ifndef VAE_NO_OGG
	#include "../../../external/tklb/src/types/audio/TVorbis.hpp"
#endif // VAE_NO_OGG

#include "../../wrapped/vae_fs.hpp"

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
			if (s.raw) {
				length = s.path.size();
			} else {
				joinedPath = path;
				joinedPath.append(s.path);
				data = joinedPath.c_str();
				fs::File file(data);
				length = file.size();
				joinedPath.reserve(length);
				if (!file.readAll(joinedPath.data())) { return Result::FileOpenError; }
				data = joinedPath.data();
			}

			if (!s.stream) {
				if (s.format == Source::Format::wav) {
				#ifdef VAE_NO_WAV
					return Result::FeatureNotCompiled;
				#else
					TKLB_PROFILER_SCOPE_NAMED("Load wav")
					bool success = tklb::wave::load<Sample, AudioBuffer>(data, length, s.signal);
					return success ? Result::Success : Result::GenericFailure;
				#endif
				} else if (s.format == Source::Format::ogg) {
				#ifdef VAE_NO_OGG
					return Result::FeatureNotCompiled;
				#else
					TKLB_PROFILER_SCOPE_NAMED("Load ogg")
					auto result = tklb::vorbis::load<Sample, AudioBuffer>(data, length, s.signal);
					return result ? Result::Success : Result::GenericFailure;
				#endif
				}
				return Result::UnsupportedFormat;
			}
			// TODO STREAM
			return Result::GenericFailure;
		}
	};
} } // vae::core


#endif // _VAE_SOURCE_LOADER
