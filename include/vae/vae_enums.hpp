#ifndef _VAE_ENUMS
#define _VAE_ENUMS

namespace vae {
	/**
	 * @brief Return Types for most engine functions
	 */
	enum class Result {
		Success = 0,				///< :)
		GenericFailure,				///< :(
		BankFormatError,			///< Generic bank loading error
		BankFormatIndexError,		///< A index is out of bounds
		BankFormatDuplicateIndex,	///< A index is used muktiple times
		FileOpenError,				///< File system could not load file
		VoiceStarvation,			///< Could not play sound because of voice limit
		BankFormatBadMixHirarchy,	///< A mixer can only write to mixers with lower ids than themselves (no recursion)
		ElementNotFound,			///< Referenced data not found
		ValidHandleRequired,		///< Handle provided wasn't valid but needs to be
		TooManyRecords,				///< Can't fit all data in fixed size array
		DeviceError,				///< Can't open audio device
		DuplicateEmitter,			///< Trying to register emitter twice
		InvalidBank,				///< Valid bank handle needed
		InvalidEmitter,				///< Emitter probably wasn't registered
		FeatureNotCompiled			///< Codec or other feature not included in binary
	};

	enum class LogLevel {
		Debug = 0,
		Info,
		Warn,
		Error,
		Ciritical
	};

	/**
	 * @brief Speaker setup for a listener
	 *
	 */
	enum class SpeakerConfiguration {
		Mono = 0,
		Headphones,
		Stereo,
		HRTF,
		Quadrophonic,
		Suround
	};
}

#endif // _VAE_ENUMS
