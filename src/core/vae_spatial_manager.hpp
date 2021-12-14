#ifndef _VAE_SPATIAL_MANAGER
#define _VAE_SPATIAL_MANAGER

#include "../../external/headeronly/robin_map.h"
#include "./pod/vae_emitter.hpp"
#include "./vae_bank_manager.hpp"
#include "./vae_voice_manager.hpp"
#include "./vae_util.hpp"
#include "./vae_types.hpp"
#include "./pod/vae_listener.hpp"

namespace vae { namespace core {
	class SpatialManager {
		// TODO for power of 2 sizes other maps might be faster and need the same amount of ram
		template <typename key, class T> using Map = tsl::robin_map<key, T>;
		Map<EmitterHandle, Emitter> mEmitters;	// All emitters across banks
		Listeners mListeners;					// All Listeners
	public:
		SpatialManager(Size emitterCount) {
			VAE_PROFILER_SCOPE
			mEmitters.reserve(emitterCount);
		}

		Result addEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE
			VAE_ASSERT(e != InvalidEmitterHandle)

			if (mEmitters.contains(e)) {
				VAE_INFO("Trying to add duplicate emitter %lu", e)
				return Result::DuplicateEmitter;
			}
			Emitter emitter;
			emitter.bank = InvalidBankHandle;
			emitter.event = InvalidEventHandle;
			// TODO VAE_DEBUG when allocation happens and also lock audio thread
			mEmitters.insert({e, emitter});
			return Result::Success;
		}

		EmitterHandle createEmitter() {
			VAE_PROFILER_SCOPE
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
			VAE_PROFILER_SCOPE
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
			VAE_PROFILER_SCOPE
			auto res = mEmitters.erase(e);
			if (res == 1) {
				return Result::Success;
			}
			return Result::ElementNotFound;
		}

		Emitter& getEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE
			return mEmitters[e];
		}

		bool hasEmitter(EmitterHandle e) {
			VAE_PROFILER_SCOPE
			return mEmitters.contains(e);
		}

		void compact() {
			VAE_PROFILER_SCOPE
			// mEmitters.compact();
		}

		Result setEmitter(
			EmitterHandle emitter, const LocationDirection& locDir,
			Sample spread
		) {
			VAE_PROFILER_SCOPE
			if (!hasEmitter(emitter)) {
				VAE_DEBUG("Accessed invalid emitter %i", emitter)
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
		void forEachListener(const Func&& func) {
			VAE_PROFILER_SCOPE
			for(ListenerHandle index = 0; index < Config::MaxListeners; index++) {
				auto& i = mListeners[index];
				if (i.id == InvalidListenerHandle) { continue; }
				func(i, index);
			}
		}

		ListenerHandle createListener() {
			VAE_PROFILER_SCOPE
			for (ListenerHandle index = 0; index < Config::MaxListeners; index++) {
				auto& i = mListeners[index];
				if (i.id == InvalidListenerHandle) {
					i.id = index;
					i.position	= { 0.f, 0.f,  0.f };
					i.front		= { 0.f, 0.f, -1.f };
					i.up		= { 0.f, 1.f,  0.f };
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
			VAE_PROFILER_SCOPE
			if (Config::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			auto& l = mListeners[listener];
			if (l.id == InvalidBankHandle) {
				return Result::ValidHandleRequired;
			}
			l.position = { locOr.position.x, locOr.position.y, locOr.position.z };
			l.front = { locOr.front.x, locOr.front.y, locOr.front.z };
			l.up = { locOr.up.x, locOr.up.y, locOr.up.z };
			return Result::Success;
		}

		Result removeListener(ListenerHandle listener) {
			VAE_PROFILER_SCOPE
			if (Config::MaxListeners <= listener) {
				VAE_WARN("Accessed invalid listener %i", listener)
				return Result::ValidHandleRequired;
			}
			mListeners[listener].id = InvalidListenerHandle;
			return Result::Success;
		}

		void update(VoiceManger& manager, BankManager& banks) {
			VAE_PROFILER_SCOPE
			manager.forEachVoice([&](Voice& v, Size vi) {
				if (v.audible) { return true; }
				if (!v.started) { return true; }
				if (!v.spatialized) { return true; }
				return false; // kill the inaudible voice
			});

			// TODO perf maybe swap loops
			forEachListener([&](Listener& l, ListenerHandle li) {
				for (auto& emitter : mEmitters) {
					auto& e = emitter.second;
					// TODO seperate auto emitter somehow
					if (e.bank == InvalidBankHandle) { continue; }
					if (e.autoplaying) { continue; }
					// only trigger sounds which haven't been auto triggered
					const auto distance = glm::distance(l.position, e.position);
					if (distance < e.maxDist) {
						mEmitters[emitter.first].autoplaying = true;
						auto& bank = banks.get(e.bank);
						manager.play(
							bank.events[e.event], e.bank, emitter.first, InvalidMixerHandle
						);
					}
				}
			});
		}
	};
} } // vae::core

#endif // _VAE_SPATIAL_MANAGER
