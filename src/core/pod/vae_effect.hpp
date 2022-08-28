#ifndef _VAE_EFFECT
#define _VAE_EFFECT

#include "../vae_types.hpp"


namespace vae { namespace core {
	namespace effect { class EffectBase; }
	/**
	 * @brief Effect pod
	 */
	struct Effect {
		/**
		 * @brief Represents a parameter of the DSP
		 */
		struct Parameter {
			IdString name;		///< name of the parameter of the effect
			Real value;			///< current value, always 0 to 1
		};
		IdString name;			///< Name of the effect to load, not an arbitrary name
		effect::EffectBase* effect = nullptr;						///< References the DSP
		Parameter parameters[StaticConfig::MaxEffectsParameter];	///< Parameters
		bool bypassed = false;
	};
} } // vae::core

#endif // _VAE_EFFECT
