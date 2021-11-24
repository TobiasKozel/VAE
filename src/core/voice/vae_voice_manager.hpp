#ifndef _VAE_VOICE_MANAGER
#define _VAE_VOICE_MANAGER

#include <vector>

#include "../vae_types.hpp"
#include "../pod/vae_source.hpp"
#include "../pod/vae_event.hpp"

#include "../../../include/vae/vae.hpp"
#include "../../../external/robin_hood.h"

namespace vae { namespace core {
	struct Voice {
		BankHandle bank = InvalidHandle;
		SourceHandle source;
		EventHandle event;
		SampleIndex time = 0;
		Time timeFract = 0.0;
		EmitterHandle emitter;
	};

	class VoiceManger {
		template <typename key, class T>
		using Map = robin_hood::unordered_flat_map<key, T>;

		Map<GenericHandle, Voice> mVoices;
		Map<GenericHandle, Voice> mVirtualVoices;
		std::vector<Voice*> mFinishedVoices;
		VoiceHandle mCurrentVoiceIndex = 0;

	public:
		VoiceManger(EngineConfig& config) {
			mVoices.reserve(config.voices);
		}

		Result play(SourceHandle source, EventHandle event, EmitterHandle emitter) {
			Voice voice;
			voice.emitter = emitter;
			voice.event = event;
			voice.source = source;
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
