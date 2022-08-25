#ifndef _VAE_UTIL
#define _VAE_UTIL

#include "../../include/vae/vae.hpp"

namespace vae { namespace core {
	constexpr void splitGlobalEventHandle(
		const GlobalEventHandle& handle,
		BankHandle& bank, EventHandle& event
	) {
		bank = handle >> (sizeof(EventHandle) * 8);
		event = (handle & InvalidEventHandle);
	}
} } // namespace vae::core

#endif // _VAE_UTIL
