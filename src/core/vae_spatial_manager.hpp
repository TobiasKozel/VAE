#ifndef _VAE_SPATIAL_MANAGER
#define _VAE_SPATIAL_MANAGER



#include "./pod/vae_emitter.hpp"
#include "./vae_bank_manager.hpp"
#include "./vae_voice_manager.hpp"
#include "./vae_util.hpp"
#include "./vae_types.hpp"
#include "./pod/vae_listener.hpp"

#define VAE_NO_EXCEPT
#ifdef VAE_NO_EXCEPT
	#define TSL_NO_EXCEPTIONS
#endif


#ifdef VAE_NO_SIMD
	#define ROBIN_HOOD_DISABLE_INTRINSICS
#endif

// TODO get rid of this since it relies on <functional> and <memory> which pull in <stdio.h>
#include "../../external/headeronly/robin_hood.h"

namespace vae { namespace core {

	class SpatialManager {

		// We do a little bit of templating
		template <typename key, class T> using Map =
			robin_hood::unordered_map<key, T>;
		// TODO for power of 2 sizes other maps might be faster and need the same amount of ram
		Map<EmitterHandle, Emitter> mEmitters;	// All emitters across banks

		Listeners mListeners;					// All Listeners
	public:
		Result init(Size emitterCount) {
			VAE_PROFILER_SCOPE_NAMED("Spatial Init")
			mEmitters.reserve(emitterCount);
			return Result::Success;
		}

		Result addEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE()
			VAE_ASSERT(e != InvalidEmitterHandle)
			if (mEmitters.contains(e)) {
				VAE_INFO("Trying to add duplicate emitter %u", e)
				return Result::DuplicateEmitter;
			}
			Emitter emitter;
			emitter.bank = InvalidBankHandle;
			emitter.event = InvalidEventHandle;
			// TODO VAE_DEBUG when allocation happens and also lock audio thread
			mEmitters.insert({e, emitter});
			VAE_PROFILER_PLOT(profiler::emitters, int64_t(mEmitters.size()));
			return Result::Success;
		}

		EmitterHandle createEmitter() {
			VAE_PROFILER_SCOPE()
			EmitterHandle ret = rand();
			while (hasEmitter(ret)) {
				ret = rand();
			}
			auto result = addEmitter(ret);
			return result == Result::Success ? ret : InvalidEmitterHandle;
		}

		EmitterHandle createAutoEmitter(
			BankHandle bank, EventHandle event, float maxDist,
			const LocationDirection& locDir, Sample spread
		) {
			VAE_PROFILER_SCOPE()
			auto handle = createEmitter();
			auto& e = mEmitters[handle];
			e.position = { locDir.position.x, locDir.position.y, locDir.position.z };
			e.spread = spread;
			e.maxDist = maxDist;
			e.bank = bank;
			e.event = event;
			return handle;
		}

		Result removeEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE()
			auto res = mEmitters.erase(e);
			if (res == 1) {
				VAE_PROFILER_PLOT(profiler::emitters, int64_t(mEmitters.size()));
				return Result::Success;
			}
			return Result::ElementNotFound;
		}

		Emitter& getEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE()
			return mEmitters[e];
		}

		bool hasEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE()
			return mEmitters.contains(e);
		}

		void compact() {
			VAE_PROFILER_SCOPE()
			// mEmitters.compact();
		}

		Result setEmitter(
			EmitterHandle emitter, const LocationDirection& locDir,
			Sample spread
		) {
			VAE_PROFILER_SCOPE()
			if (!hasEmitter(emitter)) {
				// VAE_DEBUG("Accessed invalid emitter %i", emitter)
				return Result::ElementNotFound;
			}
			auto& e = getEmitter(emitter);
			e.position = { locDir.position.x, locDir.position.y, locDir.position.z };
			e.spread = spread;
			return Result::Success;
		}

		Listeners& getListeners() {
			return mListeners;
		}

		template <class Func>
		Result forListeners(ListenerHandle handle, const Func&& func) {
			VAE_PROFILER_SCOPE()
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
			VAE_PROFILER_SCOPE()
			for (ListenerHandle index = 0; index < StaticConfig::MaxListeners; index++) {
				auto& i = mListeners[index];
				if (i.id == InvalidListenerHandle) {
					i.id = index;
					i.position	= { 0.f, 0.f,  0.f };
					i.front		= { 0.f, 0.f, -1.f };
					i.up		= { 0.f, 1.f,  0.f };
					return index;
				}
			}
			VAE_ERROR("Exeeded maxim amount of listeners define in StaticConfig::MaxListeners")
			return InvalidListenerHandle;
		}

		Result setListenerConfiguration(
			ListenerHandle listener,
			SpeakerConfiguration config
		) {
			VAE_PROFILER_SCOPE()
			if (StaticConfig::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
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
			VAE_PROFILER_SCOPE()
			if (StaticConfig::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
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
			VAE_PROFILER_SCOPE()
			if (StaticConfig::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			mListeners[listener].id = InvalidListenerHandle;
			return Result::Success;
		}

		template <class Callback>
		void update(VoiceManger& manager, Callback callback) {
			VAE_PROFILER_SCOPE_NAMED("Spatial Update")

			// TODO perf maybe swap loops
			// This triggers nearby auto emitters
			forListeners(AllListeners, [&](Listener& l) {
				for (auto& emitter : mEmitters) {
					auto& e = emitter.second;
					// TODO seperate auto emitter somehow
					if (e.bank == InvalidBankHandle) { continue; }
					// means it wants to auto emit
					if (e.autoplaying) { continue; }
					// only trigger sounds which haven't been auto triggered already to avoid duplicates
					const auto distance = glm::distance(l.position, e.position);
					if (distance < e.maxDist) {
						mEmitters[emitter.first].autoplaying = true;
						callback(e.event, e.bank, emitter.first);
					}
				}
				return Result::Success;
			});
		}
	};

	constexpr int _VAE_SIZE_SPATIAL_MANAGER = sizeof(SpatialManager);

} } // vae::core

#endif // _VAE_SPATIAL_MANAGER
