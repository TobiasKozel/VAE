#ifndef _VAE_EFFECTS_LIST
#define _VAE_EFFECTS_LIST

#include "../vae_util.hpp"
#include "./vae_effect_base.hpp"

namespace vae { namespace core { namespace effect {
	class EffectsFactory;

	class EffectsList {
		friend EffectsFactory;
		using Constructor = EffectBase* (*)();
		struct RegisteredEffect {
			const char* name		= nullptr;
			Constructor construtor	= nullptr;
		};
		RegisteredEffect mEffects[16];
		EffectsList() { }

	public:
		static EffectsList& instance() {
			static EffectsList i;
			return i;
		}

		void add(const char* name, Constructor constructor) {
			for (auto& i : mEffects) {
				if (i.name != nullptr) { continue; }
				i = { name, constructor };
				return;
			}
			VAE_ASSERT(false)// can't add effect
		}
	};
} } } // vae::core::faust

#include "./generated/reverb_gen.hpp"

#endif // _VAE_EFFECTS_LIST
