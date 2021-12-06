#ifndef _VAE_EVENT_MANAGER
#define _VAE_EVENT_MANAGER

#include "./vae_types.hpp"
#include "./vae_util.hpp"
#include "./pod/vae_bank.hpp"
#include "./vae_voice_manager.hpp"

namespace vae { namespace core {
	struct EventManager {

		static Result fireEvent(
			Bank& bank, EventHandle eventHandle,
			EmitterHandle emitterHandle, MixerHandle mixerHandle,
			VoiceManger& voiceManger, EngineConfig& config
		) {
			auto bankHandle = bank.id;
			VAE_ASSERT(eventHandle != InvalidEventHandle)
			VAE_ASSERT(eventHandle < bank.events.size())
			auto& event = bank.events[eventHandle];

			Result result;

			if (event.flags[Event::Flags::start]) {
				if (event.source != InvalidSourceHandle) {
					VAE_DEBUG_EVENT("Event %i:%i starts source %i", eventHandle, bankHandle, event.source)
					// Has source attached
					result = voiceManger.play(event, bankHandle, emitterHandle, mixerHandle);
					if (result != Result::Success) {
						// Failed to play for some reason
						return result;
					}
				}

				// Fire all other chained events
				for (auto& i : event.on_start) {
					if (i == InvalidEventHandle) { continue; }
					VAE_DEBUG_EVENT("Event %i:%i starts chained event %i", eventHandle, bankHandle, i)
					fireEvent(bank, i, emitterHandle, mixerHandle, voiceManger, config);
				}
			}

			if (event.flags[Event::Flags::stop]) {
				// TODO test stopping
				if (event.source != InvalidSourceHandle) {
					VAE_DEBUG_EVENT("Event %i:%i stops source %i", eventHandle, bankHandle, event.source)
					voiceManger.stopFromSource(event.source, emitterHandle);
				}
				for (auto& i : event.on_start) {
					if (i == InvalidEventHandle) { continue; }
					// kill every voice started from these events
					VAE_DEBUG_EVENT("Event %i:%i stops voices from event %i", eventHandle, bankHandle, i)
					voiceManger.stopFromEvent(i, emitterHandle);
				}
				if (event.mixer != Mixer::MasterMixerHandle) {
					// kill every voice in a mixer channel
					VAE_DEBUG_EVENT("Event %i:%i stops voices in mixer %i", eventHandle, bankHandle, event.mixer)
					voiceManger.stopFromMixer(event.mixer, emitterHandle);
				}
			}

			if (event.flags[Event::Flags::emit]) {
				VAE_DEBUG_EVENT("Event %i:%i emits event", eventHandle, bankHandle)
				if (config.eventCallback != nullptr) {
					EventCallbackData data;
					constexpr int as = sizeof(data);
					data.context = config.eventCallbackContext;
					data.bank = bankHandle;
					data.event = eventHandle;
					data.emitter = emitterHandle;
					config.eventCallback(&data);
				}
			}

			return Result::Success;
		}

		void update(VoiceManger& voiceManger) {

		}
	};
} } // vae::core
#endif // _VAE_EVENT_MANAGER
