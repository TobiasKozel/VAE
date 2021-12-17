
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
class EnginePimpl {
	EnginePimpl();
	EnginePimpl(const EnginePimpl&);
	~EnginePimpl() { };
public:
	static EnginePimpl* create();
	static EnginePimpl* create(const EngineConfig& config);

	_VAE_API_EXPORT void destroy();

	_VAE_API_EXPORT Result start ();

	_VAE_API_EXPORT Result stop ();

	_VAE_API_EXPORT void update ();

	_VAE_API_EXPORT Result fireEvent (
		BankHandle bank,
		EventHandle eventHandle,
		EmitterHandle emitterHandle = InvalidEmitterHandle,
		MixerHandle mixerHandle = InvalidMixerHandle
	);

	_VAE_API_EXPORT Result fireGlobalEvent (
		GlobalEventHandle globalHandle,
		EmitterHandle emitterHandle = InvalidEmitterHandle,
		MixerHandle mixerHandle = InvalidMixerHandle
	);

	_VAE_API_EXPORT Result stopEmitter (
		EmitterHandle emitter
	);

	_VAE_API_EXPORT EmitterHandle createEmitter ();

	_VAE_API_EXPORT EmitterHandle createAutoEmitter (
		BankHandle bank,
		EventHandle event,
		float maxDist,
		const LocationDirection& locDir,
		float spread
	);

	_VAE_API_EXPORT Result addEmitter (
		EmitterHandle h
	);

	_VAE_API_EXPORT Result removeEmitter (
		EmitterHandle h
	);

	_VAE_API_EXPORT Result setEmitter (
		EmitterHandle emitter,
		const LocationDirection& locDir,
		float spread
	);

	_VAE_API_EXPORT ListenerHandle createListener ();

	_VAE_API_EXPORT Result removeListener (
		ListenerHandle listener
	);

	_VAE_API_EXPORT Result setListener (
		ListenerHandle listener,
		const LocationOrientation& locOr
	);

	_VAE_API_EXPORT Result loadHRTF (
		const char* path
	);

	_VAE_API_EXPORT Result loadBank (
		const char* path
	);

	_VAE_API_EXPORT Result unloadBankFromPath (
		const char* path
	);

	_VAE_API_EXPORT Result unloadBankFromId (
		BankHandle bankHandle
	);

	_VAE_API_EXPORT void unloadAllBanks ();

	_VAE_API_EXPORT bool checkVersion (
		int major,
		int minor,
		int patch
	);

}; // class EnginePimpl

} // namespace vae
#endif // _VAE_GEN_PIMPL
