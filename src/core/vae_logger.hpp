#ifndef _VAE_LOGGER
#define _VAE_LOGGER

#include "../../include/vae/vae.hpp"

void vae_print(vae::LogLevel level, const char* message);

#ifdef VAE_NO_LOG
	VAE_DEBUG(msg, ...)
	VAE_INFO(msg, ...)
	VAE_WARN(msg, ...)
	VAE_ERROR(msg, ...)
	VAE_DEBUG_EVENT(msg, ...)
	VAE_DEBUG_VOICES(msg, ...)
#else
	#ifndef VAE_NO_STDIO
		#include <stdio.h>
		void vae_print(vae::LogLevel level, const char* message) {
			printf(message);
			printf("\n");
		}
	#endif
	#include "../wrapped/vae_profiler.hpp"
	#include <stdarg.h>
	#include <cstring>

	// TODO this might cause redefinition errors
	#define STB_SPRINTF_IMPLEMENTATION
	#include "../../external/headeronly/stb_sprintf.h"

	void vae_print_va(vae::LogLevel level, const char* format, va_list va) {
		constexpr int bufferSize = 1024;
		static bool locked = false;
		static char buffer[bufferSize];
		while(locked) { }
		locked = true;
		const int length = stbsp_vsnprintf(buffer, bufferSize, format, va);
		buffer[bufferSize - 1] = '\0';
		vae_print(level, buffer);
		VAE_PROFILER_MESSAGE(buffer, length)
		locked = false;
	}

	void vae_print_path(vae::LogLevel level, const char* path, int line, const char* format, ...) {
		if (path == nullptr) { return; }
		if (format == nullptr) { return; }

		constexpr int bufferSize = 1024;
		constexpr int fixedPathLength = 16;
		static bool locked = false;
		static char buffer[bufferSize];

		// Make all paths the same length so log is less messy
		int pathLength = strlen(path);
		if (fixedPathLength < pathLength) {
			path += (pathLength - fixedPathLength);
		}

		while(locked) { }
		locked = true;

		switch (level) {
			case vae::LogLevel::Debug:		stbsp_snprintf(buffer, bufferSize, "DEBUG | %s:%i \t| %s", path, line, format); break;
			case vae::LogLevel::Info:		stbsp_snprintf(buffer, bufferSize, " INFO | %s:%i \t| %s", path, line, format); break;
			case vae::LogLevel::Warn:		stbsp_snprintf(buffer, bufferSize, " WARN | %s:%i \t| %s", path, line, format); break;
			case vae::LogLevel::Error:		stbsp_snprintf(buffer, bufferSize, "ERROR | %s:%i \t| %s", path, line, format); break;
			case vae::LogLevel::Ciritical:	stbsp_snprintf(buffer, bufferSize, " CRIT | %s:%i \t| %s", path, line, format); break;
			default: break;
		}

		va_list va;
		va_start(va, format);
		vae_print_va(level, buffer, va);
		va_end(va);

		locked = false;
	}

	// As long as logging is enabled, error will be printed
	#define VAE_ERROR(msg, ...) vae_print_path(vae::LogLevel::Error, __FILE__, __LINE__, msg, ## __VA_ARGS__);

	#if !defined(VAE_RELEASE) || defined(VAE_FORCE_LOG)
		#define VAE_DEBUG(msg, ...) vae_print_path(vae::LogLevel::Debug, __FILE__, __LINE__, msg, ## __VA_ARGS__);
		#define VAE_INFO(msg, ...)  vae_print_path(vae::LogLevel::Info,  __FILE__, __LINE__, msg, ## __VA_ARGS__);
		#define VAE_WARN(msg, ...)  vae_print_path(vae::LogLevel::Warn,  __FILE__, __LINE__, msg, ## __VA_ARGS__);

		#ifdef VAE_LOG_VOICES
			#define VAE_DEBUG_VOICES(msg, ...) vae_print_path(vae::LogLevel::Debug, __FILE__, __LINE__, msg, ## __VA_ARGS__);
		#else
			#define VAE_DEBUG_VOICES(msg, ...)
		#endif // VAE_LOG_VOICES

		#ifdef VAE_LOG_EVENTS
			#define VAE_DEBUG_EVENT(msg, ...) vae_print_path(vae::LogLevel::Debug, __FILE__, __LINE__, msg, ## __VA_ARGS__);
		#else
			#define VAE_DEBUG_EVENT(msg, ...)
		#endif // VAE_LOG_EVENTS
	#else
		#define VAE_DEBUG(msg, ...)
		#define VAE_INFO(msg, ...)
		#define VAE_WARN(msg, ...)
		#define VAE_DEBUG_EVENT(msg, ...)
		#define VAE_DEBUG_VOICES(msg, ...)
	#endif // _NDEBUG

#endif

#endif // _VAE_LOGGER
