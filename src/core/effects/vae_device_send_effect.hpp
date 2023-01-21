#ifndef _VAE_DEVICE_SEND_EFFECT
#define _VAE_DEVICE_SEND_EFFECT

#include "../vae_types.hpp"
#include "./vae_effect_base.hpp"

namespace vae { namespace core { namespace effect {
	class DeviceSendEffect : public EffectBase {
		void init(int samplingFreq) override { (void) samplingFreq; }
		int getNumInputs() override { return 0; }
		int getNumOutputs() override {return 0; }
		void instanceClear() override { }
		void process(Effect& effect, const ScratchBuffer& in, ScratchBuffer& out) override { }

	};

	struct RegisterDeviceSendEffect {
		static EffectBase* create() {
			// TODO maybe allocate this in a pool
			auto ptr = new DeviceSendEffect();
			TKLB_PROFILER_MALLOC_L(ptr, sizeof(Reverb), vae::core::profiler::dspAllocator)
			return ptr;
		}
		RegisterDeviceSendEffect() {
			EffectsList::instance().add("Reverb", &create);
		}
	} __RegisterDeviceSendEffect;
} } } // vae::core::effect

#endif // _VAE_DEVICE_SEND_EFFECT
