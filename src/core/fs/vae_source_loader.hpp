#ifndef _VAE_SOURCE_LOADER
#define _VAE_SOURCE_LOADER

#include "../vae_config.hpp"
#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_source.hpp"

#include <string>
#include "../../../external/tklb/src/types/audio/TWaveFile.hpp"

namespace vae { namespace core {
	struct SourceLoader {
		/**
		 * @brief Loads a wav file for the resource
		 * @param s
		 * @param path
		 * @return Result
		 */
		static Result load(Source& s, std::string path) {
			VAE_PROFILER_SCOPE
			// TODO handle path always relative to some predefined root path
			path = path + s.path;
			if (s.flags[Source::Flags::preload]) {
				auto result = tklb::wave::load(path.c_str(), s.signal);
				return result ? Result::Success : Result::GenericFailure;
			}
			if (s.flags[Source::Flags::stream]) {
				VAE_ASSERT(false)
				// TODO preload a few samples from the wave file
				return Result::GenericFailure;
				s.signal.setValidSize(Config::StreamPrefetch);
				auto result = tklb::wave::load(path.c_str(), s.signal);
				return result ? Result::Success : Result::GenericFailure;
			}
			return Result::GenericFailure;
		}
	};
} } // vae::core


#endif // _VAE_SOURCE_LOADER
