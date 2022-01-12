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
#define VAE_VERSION_MAJOR 0		//< for incompatible API changes
#define VAE_VERSION_MINOR 0		//< for adding functionality in a backwards-compatible manner
#define VAE_VERSION_PATCH 1		//< for backwards-compatible bug fixes

/**
 * @brief Contains generated Event ids using generate_bank_defines.py
 * @details Header can be included and used for autocompletion when calling
 * vae::EnginePimpl::fireGlobalEvent or vae::core::Engine::fireGlobalEvent
 */
namespace vaeb { }

/**
 * @brief Contains Typedefinitions and basic structures use by the public API and internally
 * @details The types used internally like handles or static settings can be changed, but vae::EnginePimpl needs to be recompiled.
 */
namespace vae {
	using Sample = float;						///< Default sample types used where ever possible, changing this means the engine needs to be recompiled, short and int isn't working, probably needs a few changes in the pipeline
	using Size = unsigned int;					///< How the elements are addressed in the heapbuffer
	using Time = double;						///< Time sotred in seconds

	using SmallHandle		= unsigned char;
	using GenericHandle 	= unsigned short;
	using LargeHandle		= unsigned int;

	using BankHandle 			= SmallHandle;
	using EventHandle 			= GenericHandle;	///< The handle used to address events within a bank
	using SourceHandle 			= GenericHandle;
	using EmitterHandle 		= LargeHandle;
	using MixerHandle			= SmallHandle;
	using ListenerHandle		= SmallHandle;

	using GlobalEventHandle		= LargeHandle;		///< Used to globally address events, holds space for BankHandle and EventHandle
	using GlobalMixerHandle		= GenericHandle;	///< TODO
	using GlobalParameterHandle = LargeHandle;		///< TODO
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
	constexpr ListenerHandle AllListeners			= (~0) - 1; ///< Will address all listeners
	constexpr EmitterHandle InvalidEmitterHandle	= ~0;
	constexpr GlobalEventHandle InvalidGlobalEventHandle
		= InvalidEventHandle | (InvalidBankHandle << (sizeof(EventHandle) * 8));

	static_assert(
		(sizeof(BankHandle) + sizeof(EventHandle)) <= sizeof(GlobalEventHandle),
		"BankHandle combined with EventHandle needs to fit in GlobalEventHandle"
	);

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
		InvalidEmitter				///< Emitter probably wasn't registered
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

	/**
	 * @brief Basic struct describing a audio device
	 */
	struct DeviceInfo {
		int id;							///< Negative values for invalid device.
		unsigned int sampleRate = 0;	///< TODO not used?
		char name[255];					///< Device name reported from backend
		char api[4];					///< API abbreviation
		unsigned int bufferSize = 0;	///< desired bufferSize
		unsigned char channelsIn = 0;
		unsigned char channelsOut = 0;
	};

	/**
	 * @brief Public vae Vector 3 type
	 */
	struct Vector3 { float x, y, z; };

	/**
	 * @brief Emitters have a position and direction vector
	 * @details The direction vector isn't used since source can only emit unidirectional
	 */
	struct LocationDirection {
		Vector3 position;
		Vector3 direction;
	};

	/**
	 * @brief Listener uses additional up vector.
	 * This is the default coordinate system orientation.
	 * @attention Technically the can be changed but the speaker placements in vae::StaticConfig::Speakers need to be changed accordingly.
	 * @see vae::StaticConfig::Speakers
	 * @see vae::core::HRTFLoader
	 */
	struct LocationOrientation {
		Vector3 position	= { 0.f, 0.f,  0.f };
		Vector3 front		= { 0.f, 0.f, -1.f };	///< -z front @attention There's something weird going on since speakers are positve z front
		Vector3 up			= { 0.f, 1.f,  0.f };	///< Y up
	};


	/**
	 * @brief Struct containing relevant data passed
	 * to EventCallback provided in the EngineConfig.
	 */
	struct EventCallbackData {
		void* context;			///< Can point to custom context data also provided when setting the callback, ! not context based on event!
		BankHandle bank;		///< Which bank the event is from
		EventHandle event;		///< Which event
		EmitterHandle emitter;	///< Which emitter
	};

	using EventCallback = void(*)(const EventCallbackData*);

	/**
	 * @brief Settings for the engine defined
	 * at EnginePimpl::init
	 */
	struct EngineConfig {
		/**
		 * @brief Path where the bank files are located, needs to end with a trailing /
		 */
		const char* rootPath = "./";

		/**
		 * @brief Samplerate requested from device.
		 * @details If it doesn't support it, a resampler is used.
		 * Most of the audio samples used should be in this rate.
		 */
		Size internalSampleRate = 48000;

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

		static constexpr Size _preAllocatedEmitters = 1 << 14;

		/**
		 * @brief How many emitters to allocate upfront.
		 * @details Once this number is exceeded a reallocation will take place.
		 * This might cause a short audio dropout depending on the size.
		 * ! internal map allocates power of 2 sizes !
		 * This makes space for 16384 emitters.
		 * Having less emitters than amount of voices does't make too much sense
		 * since each voice will have one assigned.
		 * Unless one emitter triggers a lot of sounds.
		 */
		Size preAllocatedEmitters = _preAllocatedEmitters;

		/**
		 * @brief Hard limit on concurrent voices, can't be 0 or lower than hrtfVoices.
		 * @details Rendering 512 voices with filters and spatialization
		 * is probably pushing it for most situations and not even necessary.
		 * This is a few years old but AudioKinetic recommends something around a 100
		 * https://blog.audiokinetic.com/how-to-get-a-hold-on-your-voices-optimizing-for-cpu-part-1/
		 */
		Size voices = 512;

		/**
		 * @brief Amount of HRTF panned voices audible at any given time.
		 * Eats into the budget above.
		 */
		Size hrtfVoices = 16;

		/**
		 * @brief Hard limit on virtal voices.
		 * @details Virtualized voices will be revived as soon as possible and
		 * retain their playback position.
		 */
		Size virtualVoices = _preAllocatedEmitters;

		/**
		 * @brief Size of the voice queue for finished voices which
		 * need to trigger other events on_end when updating the engine.
		 * Too low of a value can cause these events to be discarded.
		 */
		Size finishedVoiceQueueSize = 1024;


		/**
		 * @brief Buffer size that will be requested from device.
		 * @details Higher values increase latency but reduce chanes of
		 * crackles and other artefacts. Some audio backends need
		 * higher values to work properly.
		 * The actual buffer size might vary based on the device.
		 */
		Size preferredBufferSize = 512;

		/**
		 * @brief Number of blocks/buffers to processed ahead.
		 * @details Increases latency but reduces chances of underruns
		 * since it's more forgiving to the scheduler.
		 */
		Size bufferPeriods = 3;

		/**
		 * @brief If this is true update() does not need to be called on the engine instance.
		 * @details This means events will be emitted more offen
		 * and if a lot of work is done in the EventCallback defined above,
		 * the audio thread will be blocked and underruns occur.
		 */
		bool updateInAudioThread = false;

		/**
		 * @brief If enabled, all processing and mixing will happen in the audio callback.
		 * @details This results in lower latency and one less thread running, but this
		 * isn't good practice apparently.
		 *
		 */
		bool processInBufferSwitch = false;
	};

	/**
	 * @brief Contains some fundamental Configuration needed at compile time
	 * Dynamic settings are contained in the EngineSettings struct above.
	 * @details Changes made here need recompilation
	 */
	namespace StaticConfig {
		/**
		 * @brief Maximum channel count used to pre allocate buffers
		 */
		constexpr unsigned char MaxChannels = 2;

		/**
		 * @brief Maximum block size
		 * @details Used to preallocate buffers for mixers and dsp.
		 * Higher values need more memory might play better with instruction caches
		 * but uses more memory.
		 */
		constexpr Size MaxBlock = 512;

		/**
		 * @brief How many Samples to prefetch for streaming sources
		 * @attention TODO no streaming for now might even be a runtime settings
		 */
		constexpr Size StreamPrefetch = 1024 * 8;

		/**
		 * @brief How many listeners can observe 3D voices
		 * @see vae::core::Listener::Listeners
		 */
		constexpr Size MaxListeners = 4;

		/**
		 * @brief How many effects a mixer channel can process
		 * @see vae::core::Mixer::effects
		 */
		constexpr Size MaxMixerEffects = 4;

		constexpr Size MaxEffectsParameter = 4;

		/**
		 * @brief How many chained events can fit in chain_events on the core::Event structure
		 * @see vae::core::Event::chained_events
		 */
		constexpr Size MaxChainedEvents = 4;

		/**
		 * @brief Minimum volume before sounds will skip rendering
		 */
		constexpr Sample MinVolume  = 0.01f;

		/**
		 * @brief Placement of the speakers around the listener used for SPCAP panning
		 * @details Distance to the listener is not taken into account.
		 * The subwoofer has no placement.
		 */
		namespace Speakers {
			constexpr Vector3 center		= {  0, 0, -1 };	///< Used for mono and souround setups (except quadrophonic)

			constexpr Vector3 left			= { -1, 0,  0 };	///< Used in 7.1 and Headphones
			constexpr Vector3 right			= { +1, 0,  0 };	///< Used in 7.1 and Headphones

			constexpr Vector3 frontLeft		= { -1, 0, +1 };	///< Stereo and suround setups @attention this should be negative but isn't, needs some investigation
			constexpr Vector3 frontRight	= { +1, 0, +1 };	///< Stereo and suround setups

			constexpr Vector3 rearLeft		= { -1, 0, -1 };	///< Sourund setups
			constexpr Vector3 rearRight		= { +1, 0, -1 };	///< Sourund setups
		}
	}
} // namespace vae

#endif // _VAE_API
