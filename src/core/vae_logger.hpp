#ifndef _VAE_IMPL_LOGGER
#define _VAE_IMPL_LOGGER

#include <stdarg.h>
#include <stdlib.h>

namespace vae { namespace core {
	/**
	 * Logger singleton class
	 * TODO expose the callback to public API
	 */
	class Logger {
		/**
		 * Function to format the log message and possibly print it.
		 * @param msg Message
		 * @param level Logleven, 0 for debug, higher for everything else
		 * @param channel Channel name, maximum of 10 characters
		 */
		using LogFormatFunc = void (*)(const char* channel, const char* msg, int level);

		/**
		 * Function to print formatted message.
		 * At least this needs to be provided
		 */
		using LogPrintFunc = void (*)(const char* msg);

		Logger() { }

		LogFormatFunc mFormatFunc = nullptr;
		LogPrintFunc  mPringFunc  = nullptr;

		void logFormat(const char* channel, const char* msg, int level) {
		}

		static Logger& instance() {
			static Logger logger;
			return logger;
		}
	public:
		static void debug(const char* a) {
		}

	};
} }

#endif // VAE_IMPL_LOGGER
