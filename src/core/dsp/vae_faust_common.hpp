#ifndef _VAE_FAUST_COMMON
#define _VAE_FAUST_COMMON

#include "../vae_types.hpp"
#include "./vae_effect_base.hpp"
#include "vae/vae.hpp"

#define FAUSTFLOAT Sample

namespace vae { namespace core { namespace effect {
	struct Meta { void declare(const char* key, const char* value) { }; };
	struct UI {
		Effect& effect;
		/**
		 * @brief The only important function to control the dsp code
		 *
		 * @param name Name of the parameter
		 * @param prop Pointer to the parameter
		 * @param pDefault Default value
		 * @param min minimum valid valud
		 * @param max maximum valid value
		 * @param stepSize Increments value should be altered
		 */
		void addHorizontalSlider(
			const char* name, Sample* prop, Sample pDefault,
			Sample min, Sample max, Sample stepSize
		) const {
			for (auto& i : effect.parameters) {
				if (i.name == name) {
					*prop = i.value;
					return;
				}
			}
			// node->addParameter(name, prop, pDefault, min, max, stepSize);
		}

		void openVerticalBox(const char* key) { };
		void openHorizontalBox(const char* key) { };
		void closeBox() { };
		void declare(Sample*, const char*, const char*) { };
		void addVerticalSlider(const char* name, Sample* prop, Sample pDefault, Sample min, Sample max, Sample stepSize) const { addHorizontalSlider(name, prop, pDefault, min, max, stepSize); }
		void addCheckButton(const char* name, Sample* prop) const { addHorizontalSlider(name, prop, 0, 0, 1, 1); }
		void addVerticalBargraph(const char* name, Sample* prop, Sample min, Sample max) const { };
		void addHorizontalBargraph(const char* name, Sample* prop, Sample min, Sample max) const { addVerticalBargraph(name, prop, min, max); };
	};

	/**
	 * @brief Interaface for faust DSP code
	 */
	class FaustBase : public EffectBase {
	public:
		// These three will be overridden by the generated faust code
		virtual void buildUserInterface(UI* ui_interface) = 0;

		virtual void instanceConstants(int samplingFreq) = 0; // called on init

		virtual void metadata(Meta* m) = 0;

		virtual void compute(int count, Sample** inputs, Sample** outputs) = 0;

		void process(Effect& effect, const AudioBuffer& in, AudioBuffer& out) override {
			// Update DSP values
			{
				VAE_PROFILER_SCOPE_NAMED("Update Faust DSP")
				UI ui = { effect };
				buildUserInterface(&ui);
			}

			// Convert buffers
			const Sample* ain[StaticConfig::MaxChannels];
			Sample* aout[StaticConfig::MaxChannels];
			in.getRaw(ain);
			out.getRaw(aout);

			// Do the thing
			{
				VAE_PROFILER_SCOPE_NAMED("Faust DSP")
				compute(in.validSize(), const_cast<Sample**>(ain), aout);
			}
		}
		FaustBase() { }
		virtual ~FaustBase() { }
	};
} } } // vae::core::effect

#endif // _VAE_FAUST_COMMON
