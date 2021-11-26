#ifndef _VAE_API_TYPES
#define _VAE_API_TYPES

namespace vae {
	using GenericHandle = unsigned int;
	using EventHandle 	= GenericHandle;
	using SourceHandle 	= GenericHandle;
	using BankHandle 	= GenericHandle;
	using EmitterHandle = GenericHandle;
	using VoiceHandle	= GenericHandle;
	using MixerHandle	= GenericHandle;
	constexpr GenericHandle InvalidHandle = ~0;

	/**
	 * @brief Return Types for most engine function
	 *
	 */
	enum class Result {
		Success,
		GenericFailure,
		BankFormatError,
		FileNotFound,
		VoiceStarvation,
		ElementNotFound
	};

	/**
	 * @brief Basic struct describing a audio device
	 */
	struct DeviceInfo {
		/**
		 * Negative values for invalid device.
		 */
		int id;
		unsigned int channelsIn;
		unsigned int channelsOut;
		unsigned int sampleRate;
		char name[255];
		char api[4]; // API abbreviation
		unsigned int bufferSize = 512;
	};

	/**
	 * @brief Struct containing relevant data passed
	 * to EventCallback provided in the EngineConfig.
	 */
	struct EventCallbackData {
		const char* name;		// name of the event for debugging
		void* payload;			// Can point to custom context data also provided when setting the callback
		BankHandle bank;		// Which bank the event is from
		EventHandle event;		// Which event
		EmitterHandle emitter;	// Which emitter
	};

	/**
	 * @brief Settings for the engine defined
	 * at construction of the engine object.
	 */
	struct EngineConfig {
		using EventCallback = void(*)(EventCallbackData);
		/**
		 * Each time a event of the type emit gets triggered
		 * Used to get information about ending sounds and similar
		 */
		EventCallback eventCallback = nullptr;

		/**
		 * Custom data that can be accached to the EventCallback
		 * to maintain context
		 */
		void* eventCallbackPayload = nullptr;

		/**
		 * Hard limit on concurrentvoices, can't be 0
		 */
		unsigned int voices = 1024;

		/**
		 * Hard limit on virtal voices
		 * Can be zero for no limit but will cause allocations on fireEvent
		 * If non zero value fireEvent might not return a handle
		 */
		unsigned int virtualVoices = 0;

		/**
		 * When a device is openeed. it will try to use this samplerate.
		 * If it doesn't support it, a resampler is used.
		 * This is efficient if most of the audio is in the preferred samplerate,
		 * since they don't need to be resampled.
		 */
		unsigned int preferredSampleRate = 48000;
	};
} // namespace vae

#endif // _VAE_API_TYPES
