/**
 * @file vae.hpp
 * @author Tobias Kozel (t.kozel@pm.me)
 * @brief Contains all public API types for VAE
 * @version 0.1
 * @date 2021-11-27
 *
 * @copyright Copyright (c) 2021
 *
 */


#ifndef _VAE_API_TYPES
#define _VAE_API_TYPES

namespace vae {
	using SmallHandle		= unsigned char;
	using GenericHandle 	= unsigned short;
	using EventHandle 		= GenericHandle;
	using SourceHandle 		= GenericHandle;
	using BankHandle 		= SmallHandle;
	using EmitterHandle 	= GenericHandle;
	using VoiceHandle		= GenericHandle;
	using MixerHandle		= SmallHandle;
	using ListenerHandle	= SmallHandle;

	constexpr BankHandle InvalidBankHandle			= ~0;
	constexpr MixerHandle InvalidMixerHandle		= ~0;
	constexpr ListenerHandle InvalidListenerHandle	= ~0;
	constexpr GenericHandle InvalidHandle			= ~0;

	/**
	 * @brief Return Types for most engine functions
	 */
	enum class Result {
		Success,					// :)
		GenericFailure,				// :(
		BankFormatError,			// Generic bank loading error
		BankFormatIndexError,		// A index is out of bounds
		FileOpenError,				// File system could not load file
		VoiceStarvation,			// Could not play sound because of voice limit
		BankFormatBadMixHirarchy,	// A mixer can only write to mixers with lower ids than themselves (no recursion)
		ElementNotFound,			// Referenced data not found
		ValidHandleRequired,		// Handle provided wasn't valid but needs to be
		TooManyRecords				// Can't fit all data in fixed size array
	};

	/**
	 * @brief Basic struct describing a audio device
	 */
	struct DeviceInfo {
		int id;							// Negative values for invalid device.
		unsigned int sampleRate = 0;	// TODO not used?
		char name[255];					// Device name reported from backend
		char api[4];					// API abbreviation
		unsigned int bufferSize = 0;
		unsigned char channelsIn = 0;
		unsigned char channelsOut = 0;
	};

	/**
	 * @brief Struct containing relevant data passed
	 * to EventCallback provided in the EngineConfig.
	 */
	struct EventCallbackData {
		void* context;			// Can point to custom context data also provided when setting the callback, ! not context based on event!
		BankHandle bank;		// Which bank the event is from
		EventHandle event;		// Which event
		EmitterHandle emitter;	// Which emitter if any
	};

	/**
	 * @brief Settings for the engine defined
	 * at construction of the engine object.
	 */
	struct EngineConfig {
		using EventCallback = void(*)(const EventCallbackData*);
		/**
		 * @brief Path where the bank files are located
		 */
		const char* rootPath = "./";

		/**
		 * @brief Each time a event of the type emit gets triggered
		 * Used to get information about ending sounds and similar
		 */
		EventCallback eventCallback = nullptr;

		/**
		 * @brief Custom data that can be accached to the EventCallback
		 * to maintain context
		 */
		void* eventCallbackContext = nullptr;

		/**
		 * @brief Hard limit on concurrent voices, can't be 0
		 */
		unsigned int voices = 1024;

		/**
		 * @brief Hard limit on virtal voices
		 * TODO no virtual voice system for now
		 * Can be zero for no limit but will cause allocations on fireEvent
		 * If non zero value fireEvent might not return a handle
		 */
		unsigned int virtualVoices = 0;

		/**
		 * @brief When a device is openeed. it will try to use this samplerate.
		 * If it doesn't support it, a resampler is used.
		 * This is efficient if most of the audio is in the preferred samplerate,
		 * since they don't need to be resampled.
		 */
		unsigned int preferredSampleRate = 48000;

		/**
		 * @brief Higher values increase latency but reduce chanes of
		 * crackles and other artefacts. Some audio backends need
		 * higher values to work properly.
		 * The actual buffer size might based on the device.
		 */
		unsigned int preferredBufferSize = 512;

		/**
		 * @brief Number of blocks/buffers to processed ahead.
		 * Increases latency but reduces chances of underruns
		 * since it's more forgiving to the scheduler.
		 */
		unsigned int bufferPeriods = 3;

		/**
		 * @brief If this is true update() does not need to be called on the engine instance.
		 * This means events will be emitted more offen
		 * and if a lot of work is done in the EventCallback defined above,
		 * the audio thread will be blocked and underruns occur.
		 */
		bool updateInAudioThread = false;

		/**
		 * @brief If enabled, all processing and mixing will happen in the audio callback.
		 * This results in lower latency and one less thread running, but this
		 * isn't good practice apparently.
		 *
		 */
		bool processInBufferSwitch = true;
	};
} // namespace vae

#endif // _VAE_API_TYPES
