#ifndef VAE_IMPL_LOGGER
#define VAE_IMPL_LOGGER

namespace VAE { namespace Impl {
	class Logger {
		using Callback = void (*)(const char* msg, int channel, int level);
		Logger() { }
		Callback mCustomCallback = nullptr;
		static Logger& instance() {
			static Logger logger;
			return logger;
		}
	public:

		static void setCallback(Callback callback) {
			instance().mCustomCallback = callback;
		}

	};
} }

#endif // VAE_IMPL_LOGGER
