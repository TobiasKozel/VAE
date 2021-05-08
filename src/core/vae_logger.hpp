#ifndef VAE_IMPL_LOGGER
#define VAE_IMPL_LOGGER

#include <stdarg.h>
#include <stdlib.h>

namespace vae { namespace Impl {
	class Logger {
		using Callback = void (*)(const char* msg, int channel, int level);
		Logger() { }
		Callback mCustomCallback = nullptr;
		static Logger& instance() {
			static Logger logger;
			return logger;
		}
	public:
		enum Channels {
			General = 0,
			HRTF,
			Device,
			DSP,
			Propagation,
		};

		static void setCallback(Callback callback) {
			instance().mCustomCallback = callback;
		}

		static void debug() {
		}

	};
} }

#endif // VAE_IMPL_LOGGER
