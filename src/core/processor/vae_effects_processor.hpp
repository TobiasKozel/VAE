#ifndef _VAE_EFFECTS_PROCESSOR
#define _VAE_EFFECTS_PROCESSOR

#include "../pod/vae_effect.hpp"
#include "../pod/vae_mixer.hpp"
#include "../effects/vae_effect_base.hpp"
#include "../vae_util.hpp"

namespace vae { namespace core {
	class EffectsProcessor {
		ScratchBuffer mScratchBuffer;
	public:
		void init() {
			mScratchBuffer.resize(StaticConfig::MaxBlock, StaticConfig::MaxChannels);
		}
		void mix(Effect& effect, ScratchBuffer& buffer) {
			if (effect.effect == nullptr || effect.bypassed) { return; }
			TKLB_PROFILER_SCOPE_NAMED("Process Effect")
			mScratchBuffer.set(buffer);
			mScratchBuffer.setValidSize(buffer.validSize());
			TKLB_ASSERT(effect.effect->getNumInputs() == effect.effect->getNumOutputs())
			// TKLB_ASSERT(buffer.channels() <= effect.effect->getNumInputs())
			effect.effect->process(effect, mScratchBuffer, buffer);
		}
	}; // EffectsProcessor
} } // vae::core

#endif // _VAE_EFFECTS_PROCESSOR
