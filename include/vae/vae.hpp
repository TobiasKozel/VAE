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

	enum class Result {
		Success,
		GenericFailure,
		BankFormatError,
		FileNotFound,
		ElementNotFound
	};

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

	struct EventCallbackData {
		const char* name;
		void* payload;
		BankHandle bank;
		EventHandle event;
		EmitterHandle emitter;
	};

	/**
	 * Runtime settings for the engine defined
	 * at construction of the engine object
	 */
	struct EngineConfig {
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
	};
} // namespace vae

#endif // _VAE_API_TYPES
