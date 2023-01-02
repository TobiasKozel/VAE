#ifndef _VAE_SOURCE
#define _VAE_SOURCE

#include "../vae_types.hpp"

#include <stddef.h>

namespace vae { namespace core {
	/**
	 * @brief Represents a source to play audio from
	 */
	struct Source {
		SourceHandle id = InvalidSourceHandle;

		bool stream  VAE_SMALL(1);		///< If false entire sample will be loaded in ram, there's no streaming for now
		bool resample VAE_SMALL(1);		///< Whether the sound will be resampled when loading it
		bool raw VAE_SMALL(1);			///< The data in path is a data buffer TODO not used

		enum class Format {
			wav,			///< Uses dr_wav to decode wavs
			ogg,			///< Uses stb_vorbis to decode oggs
			generator		///< Not implemented
		} format VAE_SMALL(2);

		Sample gain = 1.0;	///< Gain applied to every voice creatd frin this source
		AudioBuffer signal;	///< Audio signal itself loaded by SourceLoader
		PathString path;	///< Filesystem path
		NameString name;	///< Name for debugging

		Source() {
			stream = false;
			resample = false;
			raw = false;
		}
	};

} } // namespace vae::vore

#endif // _VAE_SOURCE
