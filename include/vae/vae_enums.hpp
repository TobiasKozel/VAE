/**
 * @file vae_enums.hpp
 * @author Tobias Kozel
 * @brief Enums used in the public API
 * @version 0.0.1
 * @date 2023-01-18
 *
 * @copyright Copyright (c) 2023
 *
 */

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
		BankFormatIndexError,		///< An index is out of bounds
		BankFormatDuplicateIndex,	///< An index is used muktiple times
		FileOpenError,				///< File system could not load file
		VoiceStarvation,			///< Could not play sound because of voice limit
		BankFormatBadMixHirarchy,	///< A mixer can only write to mixers with lower ids than themselves (no recursion)
		ElementNotFound,			///< Referenced data not found
		ValidHandleRequired,		///< Handle provided wasn't valid but needs to be
		TooManyRecords,				///< Can't fit all data in fixed size array, to adjust these limits, see StaticConfig in vae.hpp
		DeviceError,				///< Can't open audio device
		DuplicateEmitter,			///< Trying to register emitter twice. Their handles need to unqique
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
		Event = 0,		///< Event of emit fired
		Marker,			///< TODO intended to be triggered for markers in playing audio samples
		ForceFeedback,	///< TODO signaling force feedback maybe
	};
}

#endif // _VAE_ENUMS
