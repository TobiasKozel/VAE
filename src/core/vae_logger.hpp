#ifndef _VAE_LOGGER
#define _VAE_LOGGER

#ifndef _NDEBUG
	#ifdef _VAE_LOG_CONST_EXPR_FILEPATH
		namespace vae { namespace core { namespace log {
			using cstr = const char* const;
			static constexpr cstr past_last_slash(cstr str, cstr last_slash) {
				return
					*str == '\0' ? last_slash :
					*str == '/'  ? past_last_slash(str + 1, str + 1) :
					past_last_slash(str + 1, last_slash);
			}

			static constexpr cstr past_last_slash(cstr str) {
				return past_last_slash(str, str);
			}
		} } }
		#include <stdio.h>
		#define __FILENAME__ ({ constexpr ::vae::core::log::cstr sf__ {::vae::core::log::past_last_slash(__FILE__)}; sf__; })
	#else
		#include <string.h>
		#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
	#endif // _VAE_LOG_CONST_EXPR_FILEPATH

	#define VAE_DEBUG(msg, ...) printf("DEBUG\t| %s:%i \t| " msg "\n", __FILENAME__, __LINE__, ## __VA_ARGS__);
	#define VAE_INFO(msg, ...)  printf ("INFO\t| %s:%i \t| " msg "\n", __FILENAME__, __LINE__, ## __VA_ARGS__);
	#define VAE_WARN(msg, ...)  printf ("WARN\t| %s:%i \t| " msg "\n", __FILENAME__, __LINE__, ## __VA_ARGS__);
	#define VAE_ERROR(msg, ...) printf("ERROR\t| %s:%i \t| " msg "\n", __FILENAME__, __LINE__, ## __VA_ARGS__);
#else
	#define VAE_DEBUG(msg, ...) ;
	#define VAE_INFO(msg, ...)  ;
	#define VAE_WARN(msg, ...)  ;
	#define VAE_ERROR(msg, ...) ;
#endif // _NDEBUG

#endif // _VAE_LOGGER
