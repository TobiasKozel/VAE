
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
	 * @brief Initializes the engine and does most of the upfront allocations. Run this before start !
	 * @details Everything will be allocated according to the provided config.
	 *          Loading a Bank will still cause an allocation.
	 *          If there are already banks loaded, they will be reloaded to have the correct samplerate.
	 * @see start
	 * @param config Optional config to setup the internals.
	 * @return Result
	 */
	Result _VAE_API_EXPORT init (
		const EngineConfig& config = {}
	);

	/**
	 * @brief Tries to open default device and start audio thread. Call this after start.
	 * @see init
	 * @return Result
	 */
	Result _VAE_API_EXPORT start ();

	/**
	 * @brief Stops processing and waits for audio thead to clean up
	 * @return Result
	 */
	Result _VAE_API_EXPORT stop ();

	/**
	 * @brief Update function needs to be called regularly to handle outbound events and other housekeeping.
	 * @details If this isn't called regularly events might be lost and chained events not fired.
	 * When EngineConfig::updateInAudioThread is true, this doesn't need to be called manually.
	 * @see EngineConfig::updateInAudioThread
	 */
	void _VAE_API_EXPORT update ();

	/**
	 * @brief Main mechanism to start and stop sounds
	 * @see Event
	 * @param bankHandle bank id where the event is provided
	 * @param eventHandle id of the event
	 * @param emitterHandle handle of the emitter, needed for spatial audio or controlling the voice
	 * @param gain optional volume factor
	 * @param mixerHandle optional id of mixer channel sound will be routed to, this will override the one set in the event
	 * @param listenerHandle For which listener this event will be audible for, default to all
	 * @return Result
	 */
	Result _VAE_API_EXPORT fireEvent (
		BankHandle bankHandle,
		EventHandle eventHandle,
		EmitterHandle emitterHandle,
		Sample gain = 1.0,
		MixerHandle mixerHandle = InvalidMixerHandle,
		ListenerHandle listenerHandle = AllListeners
	);

	/**
	 * @brief Works like fireEvent but with a global Event identifier
	 * @see fireEvent
	 * @param globalHandle The GlobalEventHandle combines both bank and event id
	 * @param emitterHandle optional handle of the emitter, needed for spatial audio
	 * @param gain optional volume factor
	 * @param mixerHandle id of mixer channel sound will be routed to, this will override the one set in the event
	 * @return Result
	 */
	Result _VAE_API_EXPORT fireGlobalEvent (
		GlobalEventHandle globalHandle,
		EmitterHandle emitterHandle,
		Sample gain = 1.0,
		MixerHandle mixerHandle = InvalidMixerHandle,
		ListenerHandle listenerHandle = AllListeners
	);

	/**
	 * @brief Get the number of currently playing Voices
	 */
	Size _VAE_API_EXPORT getActiveVoiceCount ();

	/**
	 * @brief Get the number of currently playing virtual Voices
	 */
	Size _VAE_API_EXPORT getInactiveVoiceCount ();

	Size _VAE_API_EXPORT getStreamTime ();

	/**
	 * @brief Set the global output volume before the limiter.
	 * @details The engine can't clip, but if the output is too load
	 * the signal will be squashed in the limiter.
	 * @param volume 1.0 is the default, not interpolated for now
	 */
	void _VAE_API_EXPORT setMasterVolume (
		Sample volume
	);

	/**
	 * @brief Check if the compiled version matches
	 */
	bool _VAE_API_EXPORT checkVersion (
		int major,
		int minor,
		int patch
	);

	/**
	 * @brief Creates an emitter and returns the handle
	 * @return EmitterHandle Random handle
	 */
	EmitterHandle _VAE_API_EXPORT createEmitter ();

	/**
	 * @brief Emitter which triggers an event once a listener is close enough
	 *
	 * @param bank
	 * @param event
	 * @param maxDist
	 * @param locDir
	 * @param spread
	 * @return EmitterHandle Handle like a normal emitter
	 */
	EmitterHandle _VAE_API_EXPORT createAutoEmitter (
		BankHandle bank,
		EventHandle event,
		float maxDist,
		const EmitterProperties& locDir,
		float spread
	);

	/**
	 * @brief Unregister a emiter an kill all its voices
	 * @param h
	 * @return Result
	 */
	Result _VAE_API_EXPORT removeEmitter (
		EmitterHandle h
	);

	/**
	 * @brief Set the Emitter position, orientation and spread
	 * @param emitter The emitter
	 * @param locDir The desired location
	 * @param spread The width of the panning 0.5 is the default (only non hrtf voices)
	 * @return Result
	 */
	Result _VAE_API_EXPORT setEmitter (
		EmitterHandle emitter,
		const EmitterProperties& locDir,
		float spread
	);

	/**
	 * @brief Stop all voices from emitter.
	 * @param emitter
	 * @return Result
	 */
	Result _VAE_API_EXPORT stopEmitter (
		EmitterHandle emitter
	);

	/**
	 * @brief Sets the volume of all active voices with this emitter
	 * @param emitter
	 * @param gain
	 */
	void _VAE_API_EXPORT setVolume (
		EmitterHandle emitter,
		Sample gain
	);

	/**
	 * @brief Set the current time of all voices with the emitter.
	 * @param emitter
	 * @param time Time in samples
	 */
	void _VAE_API_EXPORT seek (
		EmitterHandle emitter,
		Size time
	);

	/**
	 * @brief Set the playback speed
	 * @param emitter
	 * @param speed 1.0 is the default speed, pitch will be affected as well.
	 */
	void _VAE_API_EXPORT setSpeed (
		EmitterHandle emitter,
		float speed
	);

	/**
	 * @brief Simple lowpass filter for the voices
	 * @param emitter
	 * @param cutoff 0-1. 0 doesn't filter, 1 filter the wholespektrum
	 */
	void _VAE_API_EXPORT setLowpass (
		EmitterHandle emitter,
		float cutoff
	);

	/**
	 * @brief Simple highpass filter for the voices
	 * @param emitter
	 * @param cutoff 0-1. 0 doesn't filter, 1 filter the wholespektrum
	 */
	void _VAE_API_EXPORT setHighpass (
		EmitterHandle emitter,
		float cutoff
	);

	/**
	 * @brief Create a Listener object
	 * @details TODO make listener 0 implicit
	 * @return ListenerHandle
	 */
	ListenerHandle _VAE_API_EXPORT createListener ();

	/**
	 * @brief Unregister listener
	 * @param listener
	 */
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
		const ListenerProperties& locOr
	);

	Result _VAE_API_EXPORT loadHRTF (
		CString path,
		Size size = 0
	);

	/**
	 * @brief Load bank from filesystem.
	 * @details This operation might take a little time but won't lock the audio thread
	 *          until the bank is inserted. This should be safe to do at any time.
	 * @param path
	 * @return Result
	 */
	Result _VAE_API_EXPORT loadBank (
		CString path,
		Size size = 0
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
	 * @brief Unloadbank by path
	 * @param path path used to load the bank
	 * @return Result
	 */
	Result _VAE_API_EXPORT unloadBank (
		CString path
	);

	/**
	 * @brief Unload every bank and data associated with it
	 */
	void _VAE_API_EXPORT unloadAllBanks ();

}; // class EnginePimpl

} // namespace vae
#endif // _VAE_GEN_PIMPL
