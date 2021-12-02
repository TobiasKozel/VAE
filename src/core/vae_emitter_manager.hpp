#ifndef _VAE_EMITTER_MANAGER
#define _VAE_EMITTER_MANAGER

#include "../../external/robin_hood.h"
#include "./pod/vae_emitter.hpp"
#include "./vae_util.hpp"
#include "./vae_types.hpp"

namespace vae { namespace core {
	struct EmitterManager {
		template <typename key, class T> using Map = robin_hood::unordered_flat_map<key, T>;
		Map<EmitterHandle, Emitter> mEmitters;

		Result addEmitter(EmitterHandle e) {
			if (mEmitters.contains(e)) {
				return Result::DuplicateEmitter;
			}
			mEmitters.insert(e, { });
			return Result::Success;
		}

		Result removeEmitter(EmitterHandle e) {
			return mEmitters.erase(e) ? Result::Success : Result::ElementNotFound;
		}
	};
} } // vae::core

#endif // _VAE_EMITTER_MANAGER
