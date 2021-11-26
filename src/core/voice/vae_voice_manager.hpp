#ifndef _VAE_VOICE_MANAGER
#define _VAE_VOICE_MANAGER

#include <vector>

#include "../vae_util.hpp"

#include "../vae_types.hpp"
#include "../pod/vae_source.hpp"
#include "../pod/vae_event.hpp"

#include "../../../include/vae/vae.hpp"
#include "../pod/vae_voice.hpp"
// #include "../../../external/robin_hood.h"

namespace vae { namespace core {
	struct VoiceManger {
		std::vector<Voice> voices;				// Currently playing voice are here
		std::vector<Voice> voicesFinished;		// voices that finished playing are here
		EventHandle currentEventInstance = 0;

		VoiceManger(EngineConfig& config) {
			voices.resize(config.voices);
			voicesFinished.resize(config.voices);
		}

		Result play(
			Event& event, BankHandle bank,
			EmitterHandle emitter, MixerHandle mixer
		) {
			// Find a free voice
			// TODO VAE PERF
			for (auto& i : voices) {
				if(i.source == InvalidHandle) {
					i = {}; // re init object resets time and so on
					i.source = event.source;
					i.event = event.id;

					if (mixer != InvalidMixerHandle && !event.force_mixer) {
						// Only use the mixer provided if it's valid
						// and the event allows overriding it
						i.mixer = mixer;
					} else {
						// Otherwise use the mixer from the event
						i.mixer = event.mixer;
					}

					i.emitter = emitter;
					i.bank = bank;
					i.eventInstance = currentEventInstance;
					currentEventInstance++;
					return Result::Success;
				}
			}

			return Result::VoiceStarvation;
		}

		Result stopFromSource(SourceHandle source, EmitterHandle emitter) {
			if (source == InvalidHandle) {
				return Result::Success;
			}
			for (auto& i : voices) {
				if(i.source == source) {
					if (emitter == InvalidHandle) {
						i.source = InvalidHandle;
					} else if (i.emitter == emitter) {
						i.source = InvalidHandle;
					}
				}
			}
			return Result::Success;
		}

		Result stopFromEvent(EventHandle event, EmitterHandle emitter) {
			for (auto& i : voices) {
				if(i.event == event) {
					if (emitter == InvalidHandle) {
						i.source = InvalidHandle;
					} else if (i.emitter == emitter) {
						i.source = InvalidHandle;
					}
				}
			}
			return Result::Success;
		}

		Result stopFromMixer(MixerHandle mixer, EmitterHandle emitter) {
			for (auto& i : voices) {
				if(i.mixer == mixer) {
					if (emitter == InvalidHandle) {
						i.source = InvalidHandle;
					} else if (i.emitter == emitter) {
						i.source = InvalidHandle;
					}
				}
			}
			return Result::Success;
		}
	};
} } // vae::core

#endif // _VAE_VOICE_MANAGER
