#ifndef _VAE_LISTENER
#define _VAE_LISTENER

#include "./vae_config.hpp"
#include "../../external/tklb/src/memory/TMemory.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/resampler/TResampler.hpp"

#include "./device/vae_device.hpp"

namespace vae { namespace core {

	class Listener {
		Device* mDevice = nullptr;

	public:
		Listener(Device* device) {
			mDevice = device;
		}

		~Listener() { }


	};
} } // namespace vae::Impl

#endif // VAE_LISTENER
