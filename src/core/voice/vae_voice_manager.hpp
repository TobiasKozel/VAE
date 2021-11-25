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
			voicesFinished.resize(config.voices * 2);
		}

		Result play(
			Event& event, BankHandle bank,
			EmitterHandle emitter, MixerHandle mixer
		) {
			// One event can trigger multiple voices
			for (auto& source : event.sources) {
				// TODO
				// start sources
				// associate event with sounds
				// fire on_end when all sounds are done

				// Find a free voice
				// TODO VAE PERF
				for (auto& i : voices) {
					if(i.source == InvalidHandle) {
						i.source = source;
						i.event = event.id;
						i.mixer = mixer;
						i.emitter = emitter;
						i.bank = bank;
						i.eventInstance = currentEventInstance;
						break;
					}
				}
			}

			currentEventInstance++;
			return Result::Success;
		}

		Result stopFromSource(SourceHandle source, EmitterHandle emitter) {
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
