#ifndef _VAE_ENUMS
#define _VAE_ENUMS

namespace vae {
	/**
	 * @brief All of the Engine Error codes
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
		FeatureNotCompiled,			///< Codec or other feature not included in binary
		UnsupportedFormat			///< Tried to load a unknown (audio) format
	};

	/**
	 * @brief TODO not used at the moment
	 */
	enum class LogLevel {
		Debug = 0,
		Info,
		Warn,
		Error,
		Ciritical
	};

	/**
	 * @brief Speaker setup for a listener.
	 *        ! Adding to this will need all bitfields which use it adjusted !
	 */
	enum class SpeakerConfiguration : unsigned int {
		Mono = 0,
		Headphones,
		Stereo,
		HRTF,
		Quadrophonic,
		Suround
	};

	/**
	 * @brief TODO event type
	 */
	enum class EventCallbackType {
		// TODO
	};
}

#endif // _VAE_ENUMS
