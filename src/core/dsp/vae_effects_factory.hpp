#ifndef _VAE_EFFECTS_FACTORY
#define _VAE_EFFECTS_FACTORY

#include "./vae_effects_list.hpp"

namespace vae { namespace core { namespace effect {
	/**
	 * @brief Class to construct effects from their name
	 *
	 */
	class EffectsFactory {
	public:
		static EffectBase* create(const IdString& id) {
			for (auto& i : EffectsList::instance().mEffects) {
				if (i.name.empty()) { continue; }
				if (id == i.name.c_str()) { return i.construtor(); }
			}
			return nullptr;
		}
	};

} } } // vae::core::effect

#endif // _VAE_EFFECTS_FACTORY
