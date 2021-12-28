#ifndef _VAE_DEFAULT_BACKEND
#define _VAE_DEFAULT_BACKEND

#ifdef _VAE_HAS_RT_AUDIO
	#define _VAE_HAS_BACKEND
	#include "./vae_rtaudio.hpp"
	namespace vae { namespace core { using DefaultBackend = BackendRtAudio; } }
#endif // _VAE_HAS_RT_AUDIO

#ifdef _VAE_HAS_PORT_AUDIO
	#define _VAE_HAS_BACKEND
	#include "./vae_portaudio.hpp"
	namespace vae { namespace core { using DefaultBackend = BackendPortAudio; } }
#endif // _VAE_HAS_PORT_AUDIO

#ifndef _VAE_HAS_BACKEND
	#define _VAE_HAS_BACKEND
	#include "./vae_device_dummy.hpp"
	namespace vae { namespace core { using DefaultBackend = BackendDummy; } }
#endif // !_VAE_HAS_BACKEND

#endif // _VAE_DEFAULT_BACKEND

