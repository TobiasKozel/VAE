#ifndef _VAE_EVENT_MANAGER
#define _VAE_EVENT_MANAGER

#include "./vae_types.hpp"
#include "./vae_util.hpp"
#include "./pod/vae_bank.hpp"
#include "./vae_voice_manager.hpp"
#include "./vae_bank_manager.hpp"

namespace vae { namespace core {
	struct EventManager {
		Result fireEvent(
			const BankHandle& bankHandle, const EventHandle& eventHandle,
			const EmitterHandle& emitterHandle, const MixerHandle& mixerHandle,
			const Sample& gain, BankManager& banks, VoiceManger& voiceManger,
			const EngineConfig& config
		) {
			VAE_PROFILER_SCOPE

			if (!banks.has(bankHandle)) {
				VAE_ERROR("Fired event %i on unloaded bank %i", eventHandle, bankHandle)
				return Result::ValidHandleRequired;
			}

			auto& bank = banks.get(bankHandle);

			if (bank.events.size() <= eventHandle) {
				VAE_WARN("Fired missing event %i on bank %i", eventHandle, bankHandle)
				return Result::ValidHandleRequired;
			}

			auto& event = bank.events[eventHandle];

			Result result;

			if (event.start) {
				if (event.source != InvalidSourceHandle) {
					VAE_DEBUG_EVENT("Event %i:%i starts source %i", eventHandle, bankHandle, event.source)
					// Has source attached
					result = voiceManger.play(event, bankHandle, gain, emitterHandle, mixerHandle);
					if (result != Result::Success) {
						// Failed to play for some reason
						return result;
					}
				}
				if (event.random) {
						for (int index = rand() % event.on_start.size(); 0 <= index; index--) {
							auto& i = event.on_start[index];
							if (i == InvalidEventHandle) { continue; }
							VAE_DEBUG_EVENT("Event %i:%i starts random event %i", eventHandle, bankHandle, i)
							fireEvent(
								bankHandle, i, emitterHandle, mixerHandle, gain,
								banks, voiceManger, config
							);
							break;
						}
				} else {
					// Fire all other chained events
					for (auto& i : event.on_start) {
						if (i == InvalidEventHandle) { continue; }
						VAE_DEBUG_EVENT("Event %i:%i starts chained event %i", eventHandle, bankHandle, i)
						fireEvent(
							bankHandle, i, emitterHandle, mixerHandle, gain,
							banks, voiceManger, config
						);
					}
				}
			}

			if (event.stop) {
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

			if (event.emit) {
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
