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

#ifndef _VAE_API
#define _VAE_API

// see https://semver.org/
#define VAE_VERSION_MAJOR 0		// for incompatible API changes
#define VAE_VERSION_MINOR 0		// for adding functionality in a backwards-compatible manner
#define VAE_VERSION_PATCH 1		// for backwards-compatible bug fixes

namespace vae {
	using SmallHandle		= unsigned char;
	using GenericHandle 	= unsigned short;
	using LargeHandle		= unsigned int;

	using BankHandle 		= SmallHandle;
	using EventHandle 		= GenericHandle;	// The handle used to address events within a bank
	using GlobalEventHandle	= LargeHandle;		// Used to globally address events, holds space for BankHandle and EventHandle
	using SourceHandle 		= GenericHandle;
	using EmitterHandle 	= LargeHandle;
	using MixerHandle		= SmallHandle;
	using ListenerHandle	= SmallHandle;

	/**
	 * @brief Since 0 is a valid handle, these are used to identify invalid ones.
	 * Seems a little odd but means they can be used to direcly address array elements.
	 */
	constexpr GenericHandle InvalidHandle			= ~0;
	constexpr EventHandle InvalidEventHandle		= ~0;
	constexpr SourceHandle InvalidSourceHandle		= ~0;
	constexpr BankHandle InvalidBankHandle			= ~0;
	constexpr MixerHandle InvalidMixerHandle		= ~0;
	constexpr ListenerHandle InvalidListenerHandle	= ~0;
	constexpr EmitterHandle InvalidEmitterHandle	= ~0;
	constexpr GlobalEventHandle InvalidGlobalEventHandle
		= InvalidEventHandle | (InvalidBankHandle << (sizeof(EventHandle) * 8));

	static_assert((sizeof(BankHandle) + sizeof(EventHandle)) <= sizeof(GlobalEventHandle));

	/**
	 * @brief Return Types for most engine functions
	 */
	enum class Result {
		Success = 0,				// :)
		GenericFailure,				// :(
		BankFormatError,			// Generic bank loading error
		BankFormatIndexError,		// A index is out of bounds
		FileOpenError,				// File system could not load file
		VoiceStarvation,			// Could not play sound because of voice limit
		BankFormatBadMixHirarchy,	// A mixer can only write to mixers with lower ids than themselves (no recursion)
		ElementNotFound,			// Referenced data not found
		ValidHandleRequired,		// Handle provided wasn't valid but needs to be
		TooManyRecords,				// Can't fit all data in fixed size array
		DeviceError,				// Can't open audio device
		DuplicateEmitter
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
	 * @brief Public vae Vector 3 type
	 */
	struct Vector3 {
		float x, y, z;
	};
	struct LocationDirection {
		Vector3 position;
		Vector3 direction;
	};

	struct LocationOrientation {
		Vector3 position	= { 0.f, 0.f,  0.f };
		Vector3 front		= { 0.f, 0.f, -1.f };
		Vector3 up			= { 0.f, 1.f,  0.f };
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

	using EventCallback = void(*)(const EventCallbackData*);

	/**
	 * @brief Settings for the engine defined
	 * at construction of the engine object.
	 */
	struct EngineConfig {
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


		static constexpr unsigned int _preAllocatedEmitters = 1 << 14;

		/**
		 * @brief How many emitters to allocate upfront.
		 * Once this number is exceeded a reallocation will take place.
		 * This might cause a short audio dropout depending on the size.
		 * ! internal map allocates power of 2 sizes !
		 * This makes space for 16384 emitters.
		 */
		unsigned int preAllocatedEmitters = _preAllocatedEmitters;

		/**
		 * @brief Samplerate requested from device.
		 * If it doesn't support it, a resampler is used.
		 * Most of the audio samples used should be in this rate.
		 */
		unsigned int internalSampleRate = 48000;

		/**
		 * @brief Buffer size that will be requested from device.
		 * Higher values increase latency but reduce chanes of
		 * crackles and other artefacts. Some audio backends need
		 * higher values to work properly.
		 * The actual buffer size might vary based on the device.
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
		bool updateInAudioThread = true;

		/**
		 * @brief If enabled, all processing and mixing will happen in the audio callback.
		 * This results in lower latency and one less thread running, but this
		 * isn't good practice apparently.
		 *
		 */
		bool processInBufferSwitch = true;
	};
} // namespace vae

#endif // _VAE_API
