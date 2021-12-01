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

#include "./vae_util.hpp"

#include "./vae_types.hpp"
#include "./pod/vae_source.hpp"
#include "./pod/vae_event.hpp"

#include "./../../include/vae/vae.hpp"
#include "./pod/vae_voice.hpp"

namespace vae { namespace core {
	struct VoiceManger {
		HeapBuffer<Voice> voices;				// Currently playing voice are here
		HeapBuffer<Voice> finishedVoiceQueue;	// voices that finished playing are queued here
		EventHandle currentEventInstance = 0;
		Size activeVoices = 0;

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
			for (auto& v : voices) {
				if(v.source == InvalidSourceHandle) {
					v = {}; // re init object resets time and so on
					v.source = event.source;
					v.event = event.id;

					for (auto& i : event.on_end) {
						v.flags[Voice::Flags::chainedEvents] =
							v.flags[Voice::Flags::chainedEvents] || (i != InvalidEventHandle);
					}

					if (mixer != InvalidMixerHandle && !event.flags[Event::Flags::force_mixer]) {
						// Only use the mixer provided if it's valid
						// and the event allows overriding it
						v.mixer = mixer;
					} else {
						// Otherwise use the mixer from the event
						v.mixer = event.mixer;
					}

					v.emitter = emitter;
					v.bank = bank;
					v.eventInstance = currentEventInstance;
					v.gain = event.gain;
					currentEventInstance++;
					activeVoices++;
					return Result::Success;
				}
			}

			VAE_WARN("Voice starvation. Can't start voice from event %i:%i", event.id, bank)

			return Result::VoiceStarvation;
		}

		/**
		 * @brief Stops a voice. Mostly used internally since
		 * other stop function provide better usage
		 * @param v Voice to stop
		 * @return Result
		 */
		Result stopVoice(Voice& v) {
			VAE_ASSERT(v.source != InvalidSourceHandle) // voice already stopped

			if (!v.flags[Voice::Flags::chainedEvents]) {
				v.source = InvalidSourceHandle; // Mark voice as free
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
			for (auto& f : finishedVoiceQueue) {
				if (f.source == InvalidSourceHandle) {
					finished = true;
					f.event = v.event;
					f.eventInstance = v.eventInstance;
					f.mixer = v.mixer;
					f.emitter = v.emitter;
					f.bank = v.bank;

					// This is set last since it marks the
					// finished voice for other threads
					f.source = v.source;
					break;
				}
			}
			v.source = InvalidSourceHandle; // Mark voice as free
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
			VAE_ASSERT(source != InvalidSourceHandle)
			for (auto& v : voices) {
				if(v.source == source) {
					if (emitter == InvalidEmitterHandle) {
						stopVoice(v);
					} else if (v.emitter == emitter) {
						stopVoice(v);
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
			VAE_ASSERT(event != InvalidEventHandle)
			for (auto& v : voices) {
				if(v.event == event) {
					if (emitter == InvalidEmitterHandle) {
						stopVoice(v);
					} else if (v.emitter == emitter) {
						stopVoice(v);
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
			VAE_ASSERT(mixer != InvalidMixerHandle)
			for (auto& v : voices) {
				if(v.mixer == mixer) {
					if (emitter == InvalidEmitterHandle) {
						stopVoice(v);
					} else if (v.emitter == emitter) {
						stopVoice(v);
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
			VAE_ASSERT(emitter != InvalidEmitterHandle)
			for (auto& v : voices) {
				if(v.emitter == emitter) {
					stopVoice(v);
				}
			}
			return Result::Success;
		}
	};
} } // vae::core

#endif // _VAE_VOICE_MANAGER
