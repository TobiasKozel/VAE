#ifndef _VAE_SPATIAL_MANAGER
#define _VAE_SPATIAL_MANAGER

#include "./pod/vae_emitter.hpp"
#include "./vae_bank_manager.hpp"
#include "./vae_voice_manager.hpp"
#include "./vae_util.hpp"
#include "./vae_types.hpp"
#include "./pod/vae_listener.hpp"
#include "./algo/vae_vec.hpp"

namespace vae { namespace core {

	class SpatialManager {
		memory::HandleBuffer<Emitter, EmitterHandle, memory::AllocatorEmitter<>> mEmitters;

		Listeners mListeners;	// All Listeners

	public:
		Result init(Size emitterCount) {
			TKLB_PROFILER_SCOPE_NAMED("Spatial Init")
			mEmitters.resize(emitterCount);
			return Result::Success;
		}

		EmitterHandle createEmitter() {
			TKLB_PROFILER_SCOPE()
			return mEmitters.create();
		}

		EmitterHandle createAutoEmitter(
			BankHandle bank, EventHandle event, float maxDist,
			const LocationDirection& locDir, Sample spread
		) {
			TKLB_PROFILER_SCOPE()
			auto handle = createEmitter();
			auto& e = *mEmitters.at(handle);
			e.position = { locDir.position.x, locDir.position.y, locDir.position.z };
			e.separation = spread;
			e.maxDist = maxDist;
			e.bank = bank;
			e.event = event;
			return handle;
		}

		Result removeEmitter(EmitterHandle e) {
			TKLB_PROFILER_SCOPE()
			auto res = mEmitters.remove(e);
			if (res) {
				TKLB_PROFILER_PLOT(profiler::emitters, int64_t(mEmitters.size()));
				return Result::Success;
			}
			return Result::ElementNotFound;
		}

		Emitter& getEmitter(EmitterHandle e) {
			TKLB_PROFILER_SCOPE()
			return *mEmitters.at(e);
		}

		bool hasEmitter(EmitterHandle e) {
			TKLB_PROFILER_SCOPE()
			return mEmitters.at(e) != nullptr;
		}

		void compact() {
			TKLB_PROFILER_SCOPE()
		}

		Result setEmitter(
			EmitterHandle emitter, const LocationDirection& locDir,
			Sample separation
		) {
			TKLB_PROFILER_SCOPE()
			if (!hasEmitter(emitter)) {
				// VAE_DEBUG("Accessed invalid emitter %i", emitter)
				return Result::ElementNotFound;
			}
			auto& e = getEmitter(emitter);
			e.position = { locDir.position.x, locDir.position.y, locDir.position.z };
			e.separation = separation;
			return Result::Success;
		}

		Listeners& getListeners() {
			return mListeners;
		}

		template <class Func>
		Result forListeners(ListenerHandle handle, const Func&& func) {
			TKLB_PROFILER_SCOPE()
			if (handle == AllListeners) {
				for(ListenerHandle index = 0; index < StaticConfig::MaxListeners; index++) {
					auto& i = mListeners[index];
					if (i.id == InvalidListenerHandle) { continue; }
					Result result = func(i);
					if (result != Result::Success) {
						return result;
					}
				}
				return Result::Success;
			}
			return func(mListeners[handle]);
		}

		ListenerHandle createListener() {
			TKLB_PROFILER_SCOPE()
			for (ListenerHandle index = 0; index < StaticConfig::MaxListeners; index++) {
				auto& i = mListeners[index];
				if (i.id == InvalidListenerHandle) {
					LocationOrientation reference; // The up and front vector are default values here
					i.id = index;
					i.position	= reference.position;
					i.front		= reference.front;
					i.up		= reference.up;
					return index;
				}
			}
			TKLB_ERROR("Exeeded maxim amount of listeners define in StaticConfig::MaxListeners")
			return InvalidListenerHandle;
		}

		Result setListenerConfiguration(
			ListenerHandle listener,
			SpeakerConfiguration config
		) {
			TKLB_PROFILER_SCOPE()
			if (StaticConfig::MaxListeners <= listener) {
				TKLB_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			auto& l = mListeners[listener];
			if (l.id == InvalidListenerHandle) {
				return Result::ValidHandleRequired;
			}
			l.configuration = config;
			return Result::Success;
		}

		Result setListener(
			ListenerHandle listener,
			const LocationOrientation& locOr
		) {
			TKLB_PROFILER_SCOPE()
			if (StaticConfig::MaxListeners <= listener) {
				TKLB_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			auto& l = mListeners[listener];
			if (l.id == InvalidListenerHandle) {
				return Result::ValidHandleRequired;
			}
			l.position = { locOr.position.x, locOr.position.y, locOr.position.z };
			l.front = { locOr.front.x, locOr.front.y, locOr.front.z };
			l.up = { locOr.up.x, locOr.up.y, locOr.up.z };
			return Result::Success;
		}

		Result removeListener(ListenerHandle listener) {
			TKLB_PROFILER_SCOPE()
			if (StaticConfig::MaxListeners <= listener) {
				TKLB_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			mListeners[listener].id = InvalidListenerHandle;
			return Result::Success;
		}

		template <class Callback>
		void update(VoiceManger& manager, Callback callback) {
			TKLB_PROFILER_SCOPE_NAMED("Spatial Update")

			// TODO perf maybe swap loops
			// This triggers nearby auto emitters
			forListeners(AllListeners, [&](Listener& l) {
				mEmitters.iterate([&](Emitter& e, const EmitterHandle handle) {
					// TODO seperate auto emitter somehow
					if (e.bank == InvalidBankHandle) { return; } // means it wants to auto emit

					if (e.started) { return; }
					// only trigger sounds which haven't been auto triggered already to avoid duplicates
					const auto distance = vector::distance(l.position, e.position);

					if (distance < e.maxDist) {
						e.started = true;
						callback(e.event, e.bank, handle);
					}
				});
				return Result::Success;
			});
		}
	};

	constexpr int _VAE_SIZE_SPATIAL_MANAGER = sizeof(SpatialManager);

} } // vae::core

#endif // _VAE_SPATIAL_MANAGER
