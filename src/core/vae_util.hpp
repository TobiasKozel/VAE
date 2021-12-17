#ifndef _VAE_UTIL
#define _VAE_UTIL

#include "../wrapped/vae_profiler.hpp"
#include "../../external/tklb/src/util/TAssert.h"
#include "../../external/tklb/src/util/TMath.hpp"
#include "./vae_logger.hpp"
#include "../../include/vae/vae.hpp"

#define VAE_ASSERT(condition) TKLB_ASSERT(condition)
namespace vae { namespace core {
	inline void splitGlobalEventHandle(
		const GlobalEventHandle& handle,
		BankHandle& bank, EventHandle& event
	) {
		bank = handle >> (sizeof(EventHandle) * 8);
		event = (handle & InvalidEventHandle);
	}
} } // namespace vae::core



#endif // _VAE_UTIL
