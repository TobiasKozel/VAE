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

#include "./vae_util.hpp"

#include "./vae_types.hpp"

#include "./pod/vae_source.hpp"
#include "./pod/vae_event.hpp"

#include "./../../include/vae/vae.hpp"
#include "./voices/vae_voice.hpp"
#include "./voices/vae_voice_filter.hpp"
#include "./voices/vae_voice_pan.hpp"
#include "./voices/vae_voice_hrtf.hpp"


namespace vae { namespace core {
	/**
	 * @brief There is only one voice pool and VAE and it's managed here.
	 * @details This class handles starting and stopping voices as well as virtualization
	 */
	class VoiceManger {
		HeapBuffer<Voice> mFinishedVoiceQueue;	///< voices that finished playing are queued here
		HeapBuffer<Voice> mVoices;				///< Currently playing voice are here
		HeapBuffer<Voice> mVirtualVoices;		///<
		HeapBuffer<VoicePan> mVoicePans;		///< Interpolation data for the panning algorithm
		HeapBuffer<VoiceFilter> mVoiceFiltered;	///< Data needed for filtering is here

		Size mActiveVoices = 0;					///< Number of currently playing voices
		Size mInactiveVoices = 0;				///< Number of currenly virtual voices
		Size mActiveHRTFVoices = 0;				///< Number of ative hrtf voices
		Size mHRTFVoiceCount = 0;				///< Number of voices reserved for hrtf
		Size mHighestVoice = 0;					///< TODO bad
		Size mHighestFinishedVoice = 0;			///< TODO bad as well

	public:
		Result init(const EngineConfig& config) {
			VAE_PROFILER_SCOPE
			mVoices.resize(config.voices);
			mFinishedVoiceQueue.resize(config.finishedVoiceQueueSize);
			mVoicePans.resize(config.voices);
			mVoiceFiltered.resize(config.voices);
			mVirtualVoices.resize(config.virtualVoices);
			mHRTFVoiceCount = config.hrtfVoices;
			return Result::Success;
		}

		HeapBuffer<Voice>& all() {
			return mVoices;
		}

		HeapBuffer<Voice>& finished() {
			return mFinishedVoiceQueue;
		}

		Size getActiveVoiceCount() const {
			return mActiveVoices;
		}

		/**
		 * @brief Callback provided to iterate voices, needs to return
		 * a bool to indicate when a voice needs to be stopped.
		 * @tparam Func
		 * @param func
		 */
		template <class Func>
		void forEachVoice(const Func&& func) {
			VAE_PROFILER_SCOPE
			for (Size index = 0; index <= mHighestVoice; index++) {
				auto& i = mVoices[index];
				if (i.source == InvalidSourceHandle) { continue; }
				if (!func(i, index)) {
					stopVoice(i); // stop the voice if callback returns false
				}
			}
		}

		template <class Func>
		void forEachFinishedVoice(const Func&& func) {
			VAE_PROFILER_SCOPE
			for (Size i = 0; i <= mHighestFinishedVoice; i++) {
				auto& v = mFinishedVoiceQueue[i];
				if (v.source == InvalidSourceHandle) { continue; }
				if (!func(v)) { continue; };
				v.source = InvalidSourceHandle; // now the finished voice is handled
			}
		}

		VoicePan& getVoicePan(Size index) {
			return mVoicePans[index];
		}

		VoiceFilter& getVoiceFilter(Size index) {
			return mVoiceFiltered[index];
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
			Event& event, const BankHandle bank,
			const Sample gain, const EmitterHandle emitter,
			const ListenerHandle listener, const MixerHandle mixer
		) {
			VAE_PROFILER_SCOPE

			Size searchStartIndex;
			Size searchEndIndex;
			bool starved;

			if (event.HRTF) {
				searchStartIndex = 0;
				searchEndIndex = mHRTFVoiceCount;
				starved = mActiveHRTFVoices == mHRTFVoiceCount;
			} else {
				searchStartIndex = mHRTFVoiceCount;
				searchEndIndex = mVoices.size();
				starved = mActiveVoices == (mVoices.size() - mHRTFVoiceCount);
			}

			// No voices left, find one to virtualize
			if(starved) {
				Size potentialVirtual = ~0;
				Sample lowestGain = 1;
				for (Size i = searchStartIndex; i < searchEndIndex; i++) {
					auto& v = mVoices[i];
					if (v.critical) { continue; } // Don't kill important voices

					if (!v.audible) {
						if (makeVirtual(v) == Result::Success) {
							searchStartIndex = i;
							break;
						}
					}

					if (v.gain < lowestGain) {
						lowestGain = v.gain;
						potentialVirtual = i;
					}
				}

				if (potentialVirtual < searchEndIndex) {
					makeVirtual(mVoices[potentialVirtual]);
					searchStartIndex = potentialVirtual;
				}
			}

			// Find a free voice
			for (Size i = searchStartIndex; i < searchEndIndex; i++) {
				auto& v = mVoices[i];
				if(v.source == InvalidSourceHandle) {
					v = { }; // re init object resets time and so on
					v.source = event.source;
					v.event = event.id;
					v.listener = listener;

					for (auto& i : event.on_end) {
						// find out if voice should trigger events on end
						v.chainedEvents = v.chainedEvents || (i != InvalidEventHandle);
					}

					if (mixer != InvalidMixerHandle && !event.force_mixer) {
						// Only use the mixer provided if it's valid
						// and the event allows overriding it
						v.mixer = mixer;
					} else {
						// Otherwise use the mixer from the event
						v.mixer = event.mixer;
					}

					v.gain = event.gain * gain;

					// v.filtered = true; // todo provide way to init the filter settings
					if (v.filtered) {
						mVoiceFiltered[i] = { };
					}

					v.emitter = emitter;
					v.spatialized = event.spatial;
					if (v.spatialized) {
						mVoicePans[i] = { };
					}
					v.HRTF = event.HRTF;
					v.bank = bank;
					if (v.HRTF) {
						mActiveHRTFVoices++;
					} else {
						mActiveVoices++;
					}
					mHighestVoice = std::max(i, mHighestVoice);
					VAE_DEBUG_VOICES("Started voice slot %i from event %i:%i\tactive: %i",
						i, event.id, bank, mActiveVoices
					)
					return Result::Success;
				}
			}

			mHighestVoice = mVoices.size() - 1;

			VAE_WARN("Voice starvation. Can't start voice from event %i:%i", event.id, bank)

			return Result::VoiceStarvation;
		}

		/**
		 * @brief makes provided voice virtual
		 * @param v
		 * @return Result
		 */
		Result makeVirtual(Voice& v) {
			for (Size i = 0; i < mVirtualVoices.size(); i++) {
				auto& slot = mVirtualVoices[i];
				if (slot.source != InvalidSourceHandle) { continue; }
				slot = v;
				v.source = InvalidSourceHandle;
				mInactiveVoices++;
				VAE_DEBUG_VOICES("Virtualized voice from event %i:%i\tactive: %i\tincative: %i",
					v.event, v.bank, mActiveVoices, mInactiveVoices
				)
				return Result::Success;
			}
			return Result::GenericFailure;
		}

		/**
		 * @brief Stops a voice. Mostly used internally since
		 * other stop function provide better usage
		 * also revives virtual voices
		 * @param v Voice to stop
		 * @return Result
		 */
		Result stopVoice(Voice& v) {
			VAE_PROFILER_SCOPE
			if (v.source == InvalidSourceHandle) { return Result::Success; }

			if (!v.chainedEvents) {
				if (0 < mInactiveVoices) {
					// If we have inactive voices, revive one
					for (Size i = 0; i < mVirtualVoices.size(); i++) {
						auto virt = mVirtualVoices[i];
						if (virt.source != InvalidSourceHandle) { continue; }
						if (virt.HRTF == v.HRTF) {
							mInactiveVoices--;
							virt.started = false;	// marks filter buffers for clearing
							v = virt;
							virt.source = InvalidSourceHandle;
							VAE_DEBUG_VOICES("Revived voice from event %i:%i\tactive: %i\tincative: %i",
								v.event, v.bank, mActiveVoices, mInactiveVoices
							)
							return Result::Success;
						}
					}
				}
				v.source = InvalidSourceHandle; // Mark voice as free
				if (v.HRTF) {
					mActiveHRTFVoices--;
				} else {
					mActiveVoices--;
				}
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
			for (Size i = 0; i < mFinishedVoiceQueue.size(); i++) {
				auto& f = mFinishedVoiceQueue[i];
				if (f.source == InvalidSourceHandle) {
					mHighestFinishedVoice = std::max(mHighestFinishedVoice, i);
					finished = true;
					f = v;
					// // This is set last since it marks the
					// // finished voice for other threads
					// f.source = v.source;
					VAE_DEBUG_VOICES("Stopped voice from event %i:%i\tactive: %i",
						f.event, f.bank, mActiveVoices
					)
					break;
				}
			}
			if (v.HRTF) {
				mActiveHRTFVoices--;
			} else {
				mActiveVoices--;
			}
			v.source = InvalidSourceHandle; // Mark voice as free

			if (!finished) {
				mHighestFinishedVoice = mFinishedVoiceQueue.size() - 1;
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
			VAE_PROFILER_SCOPE
			VAE_ASSERT(source != InvalidSourceHandle)
			for (auto& v : mVoices) {
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
			VAE_PROFILER_SCOPE
			VAE_ASSERT(event != InvalidEventHandle)
			for (auto& v : mVoices) {
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
			VAE_PROFILER_SCOPE
			VAE_ASSERT(mixer != InvalidMixerHandle)
			for (auto& v : mVoices) {
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
			VAE_PROFILER_SCOPE
			VAE_ASSERT(emitter != InvalidEmitterHandle)
			for (auto& v : mVoices) {
				if(v.emitter == emitter) {
					stopVoice(v);
				}
			}
			return Result::Success;
		}
	};

	constexpr int _VAE_SIZE_VOICE_MANAGER = sizeof(VoiceManger);
} } // vae::core

#endif // _VAE_VOICE_MANAGER
