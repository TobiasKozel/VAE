#ifndef _VAE_EFFECT_BASE
#define _VAE_EFFECT_BASE

#include "../vae_types.hpp"
#include "../pod/vae_effect.hpp"
#include "../../wrapped/vae_profiler.hpp"

namespace vae { namespace core { namespace effect {
	/**
	 * @brief Base for all effects
	 */
	class EffectBase {
	public:
		virtual void init(int samplingFreq) = 0;		///< Init dsp and clear state
		virtual int getNumInputs() = 0;
		virtual int getNumOutputs() = 0;
		virtual void instanceClear() = 0;				///< Reset dsp state delay lines and so on
		virtual void process(Effect& effect, const AudioBuffer& in, AudioBuffer& out) = 0;
		EffectBase() { }
		virtual ~EffectBase() {
			VAE_PROFILER_FREE_L(this, vae::core::profiler::dspAllocator)
		}
	};

} } } // vae::core::effect

#endif // _VAE_EFFECT_BASE
