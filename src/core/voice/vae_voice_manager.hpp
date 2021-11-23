#ifndef _VAE_VOICE_MANAGER
#define _VAE_VOICE_MANAGER

#include <vector>

#include "../vae_types.hpp"
#include "../pod/vae_source.hpp"
#include "../pod/vae_event.hpp"


#include "../../../include/vae/vae.hpp"
#include "../../../external/robin_hood.h"


namespace vae { namespace core {
	class Voice {
		SampleIndex mSampleIndex = 0;
		Time mTime = 0.0;
		EmitterHandle mEmitter;
		SourceHandle mSource;
		EventHandle mEvent;
	};

	class VoiceManger {
		template <typename key, class T>
		using Map = robin_hood::unordered_flat_map<key, T>;

		Map<GenericHandle, Voice> mVoices;
		Map<GenericHandle, Voice> mVirtualVoices;
		std::vector<Voice*> mFinishedVoices;

	public:
		VoiceManger(EngineConfig& config) {
			mVoices.reserve(config.voices);
		}

		Result play(Source& source, EventHandle event, EmitterHandle emitter) {
			return Result::GenericFailure;
		}

		Result stopFromSource(SourceHandle source, EmitterHandle emitter) {
			return Result::GenericFailure;
		}

		Result stopFromEvent(EventHandle event, EmitterHandle emitter) {
			return Result::GenericFailure;
		}
	};
} } // vae::core

#endif // _VAE_VOICE_MANAGER
