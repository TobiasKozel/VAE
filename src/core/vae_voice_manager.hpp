#ifndef _VAE_VOICE_MANAGER
#define _VAE_VOICE_MANAGER

#include "./vae_types.hpp"

#include "../../include/vae/vae.hpp"
#include "../../external/robin_hood.h"

namespace vae { namespace core {
	class Voice {
		SampleIndex mSampleIndex = 0;
		Time mTime = 0.0;
	};

	class VoiceManger {
		template <typename key, class T>
		using Map = robin_hood::unordered_flat_map<key, T>;

		Map<GenericHandle, Voice> mVoices;
		Map<GenericHandle, Voice> mVirtualVoices;

	public:
		VoiceManger(EngineConfig& config) {
			mVoices.reserve(config.voices);
		}

		Result play() {

		}
	};
} } // vae::core

#endif // _VAE_VOICE_MANAGER
