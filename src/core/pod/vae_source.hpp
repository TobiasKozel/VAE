#ifndef _VAE_SOURCE
#define _VAE_SOURCE

#include "../vae_types.hpp"

#include <stddef.h>

namespace vae { namespace core {
	struct Source {
		SourceHandle id;
		bool stream  : 1;	///< If false entire sample will be loaded in ram, there's no streaming for now
		bool resample : 1;	///< Whether the sound will be resampled when loading it
		bool raw : 1;		///< The data in path is a data buffer
		enum class Format {
			wav,			///< Uses dr_wav to decode wavs
			ogg,			///< Uses stb_vorbis to decode oggs
			generator		///< Not implemented
		} format : 2;

		Sample gain;		///< Gain applied to every voice creatd frin this source
		AudioBuffer signal;	///< Audio signal itself loaded by SourceLoader
		PathString path;	///< Filesystem path
		NameString name;	///< Name for debugging

		Source() {
			stream = false;
			resample = false;
			gain = 1.0;
			id = InvalidSourceHandle;
			raw = false;
		}
	};

	constexpr int _VAE_BUF_SIZE = sizeof(AudioBuffer);
	constexpr int _VAE_SOURCE_SIZE = sizeof(Source);
} } // namespace vae::vore

#endif // _VAE_SOURCE
