/**
 * @file vae_voice_manager.hpp
 * @author Tobias Kozel (t.kozel@pm.me)
 * @brief Holds all voices and starts/stops them
 * @version 0.1
 * @date 2021-11-28
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _VAE_VOICE_MANAGER
#define _VAE_VOICE_MANAGER

#include <vector>

#include "../vae_util.hpp"

#include "../vae_types.hpp"
#include "../pod/vae_source.hpp"
#include "../pod/vae_event.hpp"

#include "../../../include/vae/vae.hpp"
#include "../pod/vae_voice.hpp"

namespace vae { namespace core {
	struct VoiceManger {
		std::vector<Voice> voices;				// Currently playing voice are here
		std::vector<Voice> finishedVoiceQueue;	// voices that finished playing are here
		EventHandle currentEventInstance = 0;
		Uint activeVoices = 0;

		VoiceManger(EngineConfig& config) {
			voices.resize(config.voices);
			finishedVoiceQueue.resize(config.voices);
		}

		/**
		 * @brief
		 *
		 * @param event
		 * @param bank
		 * @param emitter
		 * @param mixer
		 * @return Result
		 */
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
					i.chainedEvents = !event.on_end.empty();

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
					activeVoices++;
					return Result::Success;
				}
			}

			VAE_WARN("Voice starvation. Can't start voice from event %i in bank %i", event.id, bank)

			return Result::VoiceStarvation;
		}

		/**
		 * @brief Stops a voice. Mostly used internally since
		 * other stop function provide better usage
		 * @param v Voice to stop
		 * @return Result
		 */
		Result stopVoice(Voice& v) {
			VAE_ASSERT(v.source != InvalidHandle) // voice already stopped

			if (!v.chainedEvents) {
				v.source = InvalidHandle; // Mark voice as free
				activeVoices--;
				return Result::Success;
			}
			/**
			 * If the event triggers something on_end
			 * it needs to be added to the finishedVoiceQueue
			 * array in the voice manager.
			 * The update() function on the engine will handle it
			 */

			// TODO VAE PERF
			bool finished = false;
			for (auto& i : finishedVoiceQueue) {
				if (i.source == InvalidHandle) {
					finished = true;
					i.event = v.event;
					i.eventInstance = v.eventInstance;
					i.mixer = v.mixer;
					i.emitter = v.emitter;
					i.bank = v.bank;

					// This is set last since it marks the
					// finished voice for other threads
					i.source = v.source;
					break;
				}
			}
			v.source = InvalidHandle; // Mark voice as free
			activeVoices--;

			if (!finished) {
				// Failed to find a free spot in finished voices array
				// Event will be discarded
				VAE_WARN("finishedVoiceQueue is full. Stop Event %i in bank %i discarded", v.event, v.bank)
			}
			return Result::Success;
		}

		/**
		 * @brief Stop every voice started from a source and emitter if provided
		 *
		 * @param source Source to kill
		 * @param emitter Must also match emitter if not InvalidHandle
		 * @return Result
		 */
		Result stopFromSource(SourceHandle source, EmitterHandle emitter) {
			if (source == InvalidHandle) { return Result::ValidHandleRequired; }
			for (auto& i : voices) {
				if(i.source == source) {
					if (emitter == InvalidHandle) {
						stopVoice(i);
					} else if (i.emitter == emitter) {
						stopVoice(i);
					}
				}
			}
			return Result::Success;
		}

		/**
		 * @brief Stops all voice triggered from event
		 *
		 * @param event
		 * @param emitter
		 * @return Result
		 */
		Result stopFromEvent(EventHandle event, EmitterHandle emitter) {
			if (event == InvalidHandle) { return Result::ValidHandleRequired; }
			for (auto& i : voices) {
				if(i.event == event) {
					if (emitter == InvalidHandle) {
						stopVoice(i);
					} else if (i.emitter == emitter) {
						stopVoice(i);
					}
				}
			}
			return Result::Success;	// Nothing was playing
		}

		/**
		 * @brief Kills all voices in mixer channel
		 *
		 * @param mixer
		 * @param emitter
		 * @return Result
		 */
		Result stopFromMixer(MixerHandle mixer, EmitterHandle emitter) {
			if (mixer == InvalidMixerHandle) { return Result::ValidHandleRequired; }
			for (auto& i : voices) {
				if(i.mixer == mixer) {
					if (emitter == InvalidHandle) {
						stopVoice(i);
					} else if (i.emitter == emitter) {
						stopVoice(i);
					}
				}
			}
			return Result::Success;
		}

		/**
		 * @brief Stop all voice from a emitter
		 *
		 * @param emitter
		 * @return Result
		 */
		Result stopEmitter(EmitterHandle emitter) {
			if (emitter == InvalidMixerHandle) { return Result::ValidHandleRequired; }
			for (auto& i : voices) {
				if(i.emitter == emitter) {
					stopVoice(i);
				}
			}
			return Result::Success;
		}
	};
} } // vae::core

#endif // _VAE_VOICE_MANAGER
