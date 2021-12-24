
#ifndef _VAE_GEN_PIMPL
#define _VAE_GEN_PIMPL
#include "./vae.hpp"

#if defined(_WIN32) || defined(__CYGWIN__)
	#ifdef VAE_DLL_EXPORT
		#define _VAE_API_EXPORT __declspec(dllexport) __stdcall
	#else
		#define _VAE_API_EXPORT __stdcall
	#endif
#else
	#ifdef VAE_DLL_EXPORT
		#define _VAE_API_EXPORT __attribute__((visibility("default")))
	#else
		#define _VAE_API_EXPORT
	#endif
#endif

namespace vae {
/**
 * @brief Sealed of version of vae::core::Engine
 * @details There are a few function, which aren't exported
 * due to the fact that they would pull all the dependencies like
 * tklb::AudioBuffer back in.
 * @see vae::core::Engine
 */
class EnginePimpl {
	EnginePimpl();
	EnginePimpl(const EnginePimpl&);
	~EnginePimpl() { };
public:
	static EnginePimpl* _VAE_API_EXPORT create();

	void _VAE_API_EXPORT destroy();

	/**
	 * @brief Initialized the engine and does most of the upfront allocations.
	 * @details Everything will be allocated according to the provided config.
	 * Loading a Bank will still cause an allocation.
	 * If there are already banks loaded, they will be reloaded to have the correct samplerate.
	 * @param config Optional config to setup the internals.
	 * @return Result
	 */
	Result _VAE_API_EXPORT init (
		const EngineConfig& config = {}
	);

	/**
	 * @brief Tries to open default device and start audio thread.
	 * @return Result
	 */
	Result _VAE_API_EXPORT start ();

	/**
	 * @brief Stops processing and waits for audio thead to clean up
	 * @return Result
	 */
	Result _VAE_API_EXPORT stop ();

	/**
	 * @brief Update function needs to be called regularly to handle outbound events.
	 * If this isn't called regularly events might be lost.
	 */
	void _VAE_API_EXPORT update ();

	/**
	 * @brief Main mechanism to start and stop sounds
	 *
	 * @param bankHandle bank id where the event is provided
	 * @param eventHandle id of the event
	 * @param emitterHandle handle of the emitter, needed for spatial audio or controlling the voice
	 * @param gain optional volume factor
	 * @param mixerHandle optional id of mixer channel sound will be routed to, this will override the one set in the event
	 * @param listenerHandle For which listener this event will be adible for, default to all
	 * @return Result
	 */
	Result _VAE_API_EXPORT fireEvent (
		BankHandle bankHandle,
		EventHandle eventHandle,
		EmitterHandle emitterHandle,
		float gain = 1.0,
		MixerHandle mixerHandle = InvalidMixerHandle,
		ListenerHandle listenerHandle = AllListeners
	);

	/**
	 * @brief Works like fireEvent but with a global Event identifier
	 *
	 * @param globalHandle The GlobalEventHandle combines both bank and event id
	 * @param emitterHandle optional handle of the emitter, needed for spatial audio
	 * @param gain optional volume factor
	 * @param mixerHandle id of mixer channel sound will be routed to, this will override the one set in the event
	 * @see fireEvent
	 * @return Result
	 */
	Result _VAE_API_EXPORT fireGlobalEvent (
		GlobalEventHandle globalHandle,
		EmitterHandle emitterHandle,
		float gain = 1.0,
		MixerHandle mixerHandle = InvalidMixerHandle,
		ListenerHandle listenerHandle = AllListeners
	);

	Result _VAE_API_EXPORT stopEmitter (
		EmitterHandle emitter
	);

	/**
	 * @brief Get the number of currently playing Voices
	 */
	int _VAE_API_EXPORT getActiveVoiceCount ();

	void _VAE_API_EXPORT setMasterVolume (
		float volume
	);

	EmitterHandle _VAE_API_EXPORT createEmitter ();

	EmitterHandle _VAE_API_EXPORT createAutoEmitter (
		BankHandle bank,
		EventHandle event,
		float maxDist,
		const LocationDirection& locDir,
		float spread
	);

	Result _VAE_API_EXPORT addEmitter (
		EmitterHandle h
	);

	Result _VAE_API_EXPORT removeEmitter (
		EmitterHandle h
	);

	Result _VAE_API_EXPORT setEmitter (
		EmitterHandle emitter,
		const LocationDirection& locDir,
		float spread
	);

	ListenerHandle _VAE_API_EXPORT createListener ();

	Result _VAE_API_EXPORT removeListener (
		ListenerHandle listener
	);

	/**
	 * @brief Set the position of a listener
	 * @param listener
	 * @return Result
	 */
	Result _VAE_API_EXPORT setListener (
		ListenerHandle listener,
		const LocationOrientation& locOr
	);

	Result _VAE_API_EXPORT loadHRTF (
		const char* path
	);

	/**
	 * @brief Load bank from filesystem.
	 * @details This operation might take a little time but won't lock the audio thread
	 * until the bank is inserted. This should be safe to do at any time.
	 * @param path
	 * @return Result
	 */
	Result _VAE_API_EXPORT loadBank (
		const char* path
	);

	/**
	 * @brief Unload bank from handle.
	 * Locks audio thread and stops all voices from that bank.
	 * @param bankHandle
	 * @return Result
	 */
	Result _VAE_API_EXPORT unloadBankFromId (
		BankHandle bankHandle
	);

	/**
	 * @brief Unload every bank and data associated with it
	 */
	void _VAE_API_EXPORT unloadAllBanks ();

	/**
	 * @brief Check if the compiled version matches
	 */
	bool _VAE_API_EXPORT checkVersion (
		int major,
		int minor,
		int patch
	);

}; // class EnginePimpl

} // namespace vae
#endif // _VAE_GEN_PIMPL
