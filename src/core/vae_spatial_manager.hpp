#ifndef _VAE_SPATIAL_MANAGER
#define _VAE_SPATIAL_MANAGER

#include "../../external/robin_hood.h"
#include "./pod/vae_emitter.hpp"
#include "./vae_util.hpp"
#include "./vae_types.hpp"
#include "./pod/vae_listener.hpp"

namespace vae { namespace core {
	class SpatialManager {
		template <typename key, class T> using Map = robin_hood::unordered_flat_map<key, T>;
		Map<EmitterHandle, Emitter> mEmitters;	// All emitters across banks
		Listeners mListeners;					// All Listeners
	public:
		SpatialManager(Size emitterCount) {
			mEmitters.reserve(emitterCount);
		}

		Result addEmitter(EmitterHandle e) {
			VAE_ASSERT(e != InvalidEmitterHandle)

			if (mEmitters.contains(e)) {
				VAE_INFO("Trying to add duplicate emitter %lu", e)
				return Result::DuplicateEmitter;
			}
			Emitter emitter;
			// TODO VAE_DEBUG when allocation happens
			mEmitters.insert({e, emitter});
			return Result::Success;
		}

		EmitterHandle createEmitter() {
			EmitterHandle ret = rand();
			while (hasEmitter(ret)) {
				ret = rand();
			}
			auto result = addEmitter(ret);
			return result == Result::Success ? ret : InvalidEmitterHandle;
		}

		Result removeEmitter(EmitterHandle e) {
			auto res = mEmitters.erase(e);
			if (res == 1) {
				return Result::Success;
			}
			return Result::ElementNotFound;
		}

		Emitter& getEmitter(EmitterHandle e) {
			return mEmitters[e];
		}

		bool hasEmitter(EmitterHandle e) {
			return mEmitters.contains(e);
		}

		void compact() {
			mEmitters.compact();
		}

		Result setEmitter(
			EmitterHandle emitter, const LocationDirection& locDir,
			Sample spread
		) {
			if (!hasEmitter(emitter)) {
				VAE_DEBUG("Accessed invalid emitter %i", emitter)
				return Result::ElementNotFound;
			}
			auto& e = getEmitter(emitter);
			e.position = { locDir.position.x, locDir.position.y, locDir.position.z };
			e.direction = { locDir.direction.x, locDir.direction.y, locDir.direction.z };
			e.spread = spread;
			return Result::Success;
		}

		Listeners& getListeners() {
			return mListeners;
		}

		template <class Func>
		void forEachListener(const Func&& func) {

			for(ListenerHandle index = 0; index < Config::MaxListeners; index++) {
				auto& i = mListeners[index];
				if (i.id == InvalidListenerHandle) { continue; }
				func(i, index);
			}
		}

		ListenerHandle createListener() {
			for (ListenerHandle index = 0; index < Config::MaxListeners; index++) {
				auto& i = mListeners[index];
				if (i.id == InvalidListenerHandle) {
					i.id = index;
					return index;
				}
			}
			VAE_ERROR("Exeeded maxim amount of listeners define in Config::MaxListeners")
			return InvalidListenerHandle;
		}

		Result setListener(
			ListenerHandle listener,
			const LocationOrientation& locOr
		) {
			if (Config::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			auto& l = mListeners[listener];
			l.postion = { locOr.position.x, locOr.position.y, locOr.position.z };
			l.front = { locOr.front.x, locOr.front.y, locOr.front.z };
			l.up = { locOr.up.x, locOr.up.y, locOr.up.z };
			return Result::Success;
		}

		Result removeListener(ListenerHandle listener) {
			if (Config::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			mListeners[listener].id = InvalidListenerHandle;
			return Result::Success;
		}

		void update() {
			// TODO virtualize voice no listener is nearby
			// TODO reactive voice when getting close again
		}
	};
} } // vae::core

#endif // _VAE_SPATIAL_MANAGER