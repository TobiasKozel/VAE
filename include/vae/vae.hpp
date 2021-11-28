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
	using GenericHandle = unsigned short;
	using EventHandle 	= GenericHandle;
	using SourceHandle 	= GenericHandle;
	using BankHandle 	= unsigned char;
	using EmitterHandle = GenericHandle;
	using VoiceHandle	= GenericHandle;
	using MixerHandle	= unsigned char;

	constexpr BankHandle InvalidBankHandle		= ~0;
	constexpr MixerHandle InvalidMixerHandle	= ~0;
	constexpr GenericHandle InvalidHandle		= ~0;

	/**
	 * @brief Return Types for most engine function
	 *
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
		ValidHandleRequired			// Handle provided wasn't valid but needs to be
	};

	/**
	 * @brief Basic struct describing a audio device
	 */
	struct DeviceInfo {
		/**
		 * Negative values for invalid device.
		 */
		int id;
		unsigned int sampleRate = 0;
		char name[255];
		char api[4]; // API abbreviation
		unsigned int bufferSize = 0;
		unsigned char channelsIn = 0;
		unsigned char channelsOut = 0;
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
		using EventCallback = void(*)(const EventCallbackData*);
		/**
		 * @brief Each time a event of the type emit gets triggered
		 * Used to get information about ending sounds and similar
		 */
		EventCallback eventCallback = nullptr;

		/**
		 * @brief Custom data that can be accached to the EventCallback
		 * to maintain context
		 */
		void* eventCallbackPayload = nullptr;

		/**
		 * @brief Hard limit on concurrentvoices, can't be 0
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

		unsigned int preferredBufferSize = 512;
	};
} // namespace vae

#endif // _VAE_API_TYPES
