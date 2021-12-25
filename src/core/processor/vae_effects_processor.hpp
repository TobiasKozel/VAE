#ifndef _VAE_EFFECTS_PROCESSOR
#define _VAE_EFFECTS_PROCESSOR

#include "../pod/vae_effect.hpp"
#include "../pod/vae_mixer.hpp"
#include "../dsp/vae_effect_base.hpp"
#include "../vae_util.hpp"

namespace vae { namespace core {
	class EffectsProcessor {
		AudioBuffer mScratchBuffer;
	public:
		void init() {
			mScratchBuffer.resize(StaticConfig::MaxBlock, StaticConfig::MaxChannels);
		}
		void mix(Effect& effect, AudioBuffer& buffer) {
			if (StaticConfig::MaxMixerEffects <= effect.index) { return; }
			VAE_PROFILER_SCOPE_NAMED("Process Effect")
			mScratchBuffer.set(buffer);
			mScratchBuffer.setValidSize(buffer.validSize());
			VAE_ASSERT(effect.effect->getNumInputs() == effect.effect->getNumOutputs())
			VAE_ASSERT(buffer.channels() <= effect.effect->getNumInputs())
			effect.effect->process(effect, mScratchBuffer, buffer);
		}
	}; // EffectsProcessor
} } // vae::core

#endif // _VAE_EFFECTS_PROCESSOR
