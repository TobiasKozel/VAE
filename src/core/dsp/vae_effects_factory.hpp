#ifndef _VAE_EFFECTS_FACTORY
#define _VAE_EFFECTS_FACTORY

#include "./vae_effects_list.hpp"

namespace vae { namespace core { namespace effect {

	class EffectsFactory {
	public:
		static EffectBase* create(IdString& id) {
			for (auto& i : EffectsList::instance().mEffects) {
				if (i.name == nullptr) { continue; }
				if (id == i.name) {
					return i.construtor();
				}
			}
			return nullptr;
		}
	};

} } } // vae::core::faust

#endif // _VAE_EFFECTS_FACTORY
