#ifndef _VAE_STRUCTS
#define _VAE_STRUCTS

#include "./vae_type_defs.hpp"

namespace vae {
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
		 * @todo If it's 0 it should use the preferred device samplerate
		 */
		Size internalSampleRate = 48000;

		/**
		 * @brief Each time a event of the type emit gets triggered
		 * Used to get information about ending sounds and similar
		 */
		void(*eventCallback)(const EventCallbackData*) = nullptr;

		/**
		 * @brief Custom data that can be accached to the EventCallback
		 * to maintain context
		 */
		void* eventCallbackContext = nullptr;

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
		Size preAllocatedEmitters = 16384;

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
		Size virtualVoices = 16384;

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
	 * @brief Basic struct describing a audio device
	 */
	struct DeviceInfo {
		int id;							///< Negative values for invalid device.
		Size sampleRate = 0;	///< TODO not used?
		char name[255];					///< Device name reported from backend
		char api[4];					///< API abbreviation
		Size bufferSize = 0;	///< desired bufferSize
		unsigned char channelsIn = 0;
		unsigned char channelsOut = 0;
	};

	/**
	 * @brief Public vae Vector 3 type
	 */
	struct Vector3 {
		float x;
		float y;
		float z;
	};

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

}

#endif // _VAE_STRUCTS
