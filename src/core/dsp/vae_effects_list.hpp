#ifndef _VAE_EFFECTS_LIST
#define _VAE_EFFECTS_LIST

#include "../vae_util.hpp"
#include "./vae_effect_base.hpp"

namespace vae { namespace core { namespace effect {
	class EffectsFactory;

	/**
	 * @brief List to register effects to.
	 *        Hardcoded to 16 effects at the moment.
	 */
	class EffectsList {
		friend EffectsFactory;
		using Constructor = EffectBase* (*)();
		struct RegisteredEffect {
			IdString name;						///< Name used to construct effect, must not exceed
			Constructor construtor	= nullptr;	///< Constructor functoin
		};
		RegisteredEffect mEffects[16];			///< Hard coded array with 16 elements
		EffectsList() { }

	public:
		static EffectsList& instance() {
			static EffectsList i;
			return i;
		}

		/**
		 * @brief Register effect here
		 * @param name Name used to construct later, This is cropped to IdString length!
		 * @param constructor Function pointer to constructor function
		 */
		void add(const char* name, Constructor constructor) {
			for (auto& i : mEffects) {
				if (i.construtor != nullptr) { continue; }	// Skip non emty elements
				i = { name, constructor };					// Add element
				return;
			}
			TKLB_ASSERT(false)// can't add effect because no space left
		}
	};
} } } // vae::core::effect

#ifndef VAE_NO_EFFECTS
	/**
	 * Header only effects are included here
 	 */
	#include "./generated/reverb_gen.hpp"
#endif // VAE_NO_EFFECTS

#endif // _VAE_EFFECTS_LIST
