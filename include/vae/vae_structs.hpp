/**
 * @file vae_structs.hpp
 * @author Tobias Kozel
 * @brief Structs used in the public api
 * @version 0.0.1
 * @date 2023-01-18
 *
 * @copyright Copyright (c) 2023
 *
 */


#ifndef _VAE_STRUCTS
#define _VAE_STRUCTS

#include "./vae_type_defs.hpp"
#include "./vae_enums.hpp"

namespace vae {

	// -----   Below are structs meant only for direct API communication   -----

	/**
	 * @brief Public vae Vector 3 type
	 *        OpenGL Coordinate System
	 */
	struct Vector3 {
		Position x = 0;		///< Right
		Position y = 0;		///< Up
		Position z = 0;		///< Back
	};

	/**
	 * @brief Emitters have a position and direction vector
	 */
	struct EmitterProperties {
		Vector3 position;
		Vector3 velocity;
	};

	/**
	 * @brief Listener uses additional up vector like a normal camera.
	 *        This is the OpenGL coordinate system orientation.
	 * @attention Technically they can be changed but the speaker placements
	 *            in vae::StaticConfig::Speakers need to be changed accordingly as well. (also not tested yet)
	 * @see vae::StaticConfig::Speakers
	 * @see vae::core::HRTFLoader
	 */
	struct ListenerProperties {
		Vector3 position;
		Vector3 front		= { 0,  0, -1 };	///< -z front
		Vector3 up			= { 0, +1,  0 };	///< +y up
		Vector3 velocity;
	};

	/**
	 * @brief Struct containing relevant data passed
	 * to EventCallback provided in the EngineConfig.
	 */
	struct EventCallbackData {
		Pointer context;		///< Can point to custom context data also provided when setting the callback, ! not context based on event!
		BankHandle bank;		///< Which bank the event is from
		EventHandle event;		///< Which event
		EmitterHandle emitter;	///< Which emitter
		EventCallbackType type; ///< TODO
	};

	/**
	 * @brief Settings for the engine provided at initialization
	 */
	struct EngineConfig {
		/**
		 * @brief Path relative to the main application pointing to the root bank file
		 */
		CString rootBankPath = "./root.json";

		/**
		 * @brief Each time a event of the type emit gets triggered
		 *        Used to get information about ending sounds and similar
		 */
		void(*eventCallback)(const EventCallbackData*) = nullptr;

		/**
		 * @brief Custom data that can be accached to the EventCallback
		 *        to maintain context
		 */
		Pointer eventCallbackContext = nullptr;

		/**
		 * @brief How many emitters to allocate upfront.
		 * @details Once this number is exceeded a reallocation will take place.
		 *          This might cause a short audio dropout depending on the size.
		 *          The maximum number is only capped by the EmitterHandle addressable space and memory.
		 *          Having less emitters than amount of voices does't make too much sense
		 *          since each voice will have one assigned.
		 *          Unless one emitter triggers a lot of sounds.
		 */
		Size preAllocatedEmitters = 16384;

		/**
		 * @brief If this is true update() does not need to be called on the engine instance.
		 * @details This means events will be emitted more offen
		 *          and if a lot of work is done in the EventCallback defined above,
		 *          the audio thread will be blocked and underruns may occur.
		 */
		bool updateInAudioThread = false;
	};

	/**
	 * @brief Basic struct describing a audio device
	 *        TODO devices aren't exposed/functional yet
	 */
	struct DeviceInfo {
		int id;							///< Negative values for invalid device.
		Size sampleRate = 0;			///< TODO not used?
		char name[255];					///< Device name reported from backend
		char api[4];					///< API abbreviation
		Size bufferSize = 0;			///< desired bufferSize
		ChannelIndex channelsIn = 0;
		ChannelIndex channelsOut = 0;
	};



	// -----   Below are serializable structs which can be loaded from the filesystem   -----
	//         TODO They can also be used from the API


	/**
	 * @brief There is only one root bank which can define
	 *        global runtime settings like the sample rate and also
	 *        set up the master mixing chain
	 */
	struct RootBank {
		/**
		 * @brief Samplerate used for processing and mixing
		 * @details If the audio device doesn't support it, a resampler is used.
		 *          Most of the audio samples used should be in this rate
		 *          to avoid loadtime and runtime resampling.
		 * @todo If it's 0 it should use the preferred device samplerate
		 */
		Size internalSampleRate = 48000;

		/**
		 * @brief This is the name which is used to register a client
		 *        to the OS Audio-Subsytem (PulseAudio, WASAPI, etc)
		 *        and which will be show in the Systems audio Mixer.
		 *        Might not be supported by the underlying Audio API.
		 */
		CString applicationName = "VAE_Client";


		/**
		 * @brief Buffer size that will be requested from device.
		 * @details Higher values increase latency but reduce chanes of
		 *          crackles and other artefacts. Some audio backends need
		 *          higher values to work properly.
		 *          The actual buffer size might vary based on the device.
		 */
		Size preferredBufferSize = 512;

		/**
		 * @brief Number of blocks/buffers to processed ahead.
		 * @details Increases latency but reduces chances of underruns
		 *          since it's more forgiving to the OS scheduler.
		 */
		Size bufferPeriods = 3;

		/**
		 * @brief If enabled, all processing and mixing will happen in the audio callback.
		 * @details This results in lower latency and one less thread running, but this
		 *          isn't good practice depending on the Audio System.
		 */
		bool processInBufferSwitch = false;

		// Also contains Mixers
	};

	/**
	 * @brief This is a normal bank of which any number addressable by @see BankHandle
	 *        can exist at any given time.
	 */
	struct Bank {
		/**
		 * @brief Globally unique id for the bank
		 */
		BankHandle id;

		/**
		 * @brief Hard limit on concurrent voices, for pre allocation. Can't be 0 or lower than hrtfVoices.
		 * @details Rendering 512 voices with filters and spatialization
		 *          is probably pushing it for most situations and not even necessary.
		 *          This is a few years old but AudioKinetic recommends something around a 100
		 *          https://blog.audiokinetic.com/how-to-get-a-hold-on-your-voices-optimizing-for-cpu-part-1/
		 */
		Size voices = 128;

		/**
		 * @brief Amount of HRTF panned voices audible at any given time.
		 *        Eats into the budget above.
		 */
		Size hrtfVoices = 16;

		/**
		 * @brief Hard limit on virtal voices.
		 * @details Virtualized voices will be revived as soon as possible and
		 *          retain their playback position.
		 */
		Size virtualVoices = 1024;

		/**
		 * @brief Size of the voice queue for finished voices which
		 *        need to trigger other events on_end when updating the engine.
		 *        Too low of a value can cause these events to be discarded.
		 */
		Size finishedVoiceQueueSize = 512;

		/**
		 * @brief Name for debugging purposes
		 */
		CString name;
	};

	struct EffectParameter {
		/**
		 * @brief Name to specify the parameter
		 *        TODO maybe use another machnism
		 */
		CString name;

		/**
		 * @brief Value of the parameter
		 *
		 */
		Sample value;
	};

	/**
	 * @brief Effects which can be processed by a @see Mixer
	 */
	struct Effect {
		/**
		 * @brief Name to specify the effect
		 *        TODO maybe use another mechanism
		 */
		CString name;

		/**
		 * @brief Whether the effect is bypassed
		 */
		bool bypassed = false;
	};


	/**
	 * @brief A Mixer which voices can mix to.
	 * @details Each bank has at least one Master mixer with the ID 0.
	 *          If not defined it's implicitly created.
	 *          Each Mixer can hold @see MaxMixerEffects of effects
	 */
	struct Mixer {
		MixerHandle id;

		/**
		 * @brief Which mixer the channel mixes to inside the bank.
		 * @warning Mixing is done from high to low, meaning recursion isn't possible.
		 *          and the parent id has to be lower than @see id
		 */
		MixerHandle parent;

		/**
		 * @brief Gain applied post effects.
		 */
		Sample gain = 1.0;

		/**
		 * @brief Name for debugging.
		 */
		CString name;
	};

	/**
	 * @brief A audio source, currently only files are supported.
	 */
	struct Source {
		/**
		 * @brief Bank unique id
		 */
		SourceHandle id;


		/**
		 * @brief Whether the audio should be streamed or loaded in at one.
		 */
		bool stream = false;

		/**
		 * @brief Whether the file should be pre resampled at load time.
		 * @warning only works when @see stream is false.
		 */
		bool resample = false;

		/**
		 * @brief Playback volume
		 */
		Sample gain = 1.0;

		// TODO format

		/**
		 * @brief Where the file is located relative to the bank
		 */
		CString path;

		/**
		 * @brief Name for debugging.
		 */
		CString name;
	};

	/**
	 * @brief Event signaling some occurence in the main application.
	 */
	struct Event {
		/**
		 * @brief Bank unique id
		 */
		EventHandle id;
		/**
		 * @brief The source which is triggered.
		 */
		SourceHandle source;
		/**
		 * @brief Where the voice should mix to
		 */
		MixerHandle mixer;

		Sample gain = 1.0;

		/**
		 * @brief Name for debugging
		 */
		CString name;
	};

	/**
	 * @brief Emitter
	 *        TODO
	 */
	struct Emitter {
		/**
		 * @brief Every emitter has a position, even if it's not spatial.
		 */
		Vector3 position;

		/**
		 * @brief how much seperation between speaker will be when applying the panning law
		 *        TODO maybe this should be in the event?
		 */
		Sample separation = 1.0;
	};
}

#endif // _VAE_STRUCTS
