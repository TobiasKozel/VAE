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
			// TODO handle path always relative to some predefined root path
			constexpr int tes = sizeof(Source);
			if (s.flags[Source::Flags::preload]) {
				path += s.path;
				auto result = tklb::wave::load(path.c_str(), s.signal);
				return result ? Result::Success : Result::GenericFailure;
			}
			if (s.flags[Source::Flags::stream]) {
				return Result::GenericFailure;
				// TODO preload a few samples from the wave file
				path += s.path;
				s.signal.setValidSize(Config::StreamPrefetch);
				auto result = tklb::wave::load(path.c_str(), s.signal);
				return result ? Result::Success : Result::GenericFailure;
			}
			return Result::GenericFailure;
		}
	};
} } // vae::core


#endif // _VAE_SOURCE_LOADER
