#ifndef _VAE_FAUST_COMMON
#define _VAE_FAUST_COMMON

#include "../vae_types.hpp"

#define FAUSTFLOAT Sample

namespace vae { namespace core { namespace faust {
struct Meta {
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
};

struct UI {

    UI() {}
    virtual ~UI() {}

    // -- widget's layouts

    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets

    virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init,
                                   FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init,
                                     FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init,
                             FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

    // -- metadata declarations

    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
};

class FaustBase {

public:

	FaustBase() {}
	virtual ~FaustBase() {}

	/* Return instance number of audio inputs */
	virtual int getNumInputs() = 0;

	/* Return instance number of audio outputs */
	virtual int getNumOutputs() = 0;

	/**
	 * Trigger the ui_interface parameter with instance specific calls
	 * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
	 *
	 * @param ui_interface - the user interface builder
	 */
	virtual void buildUserInterface(UI* ui_interface) = 0;

	/* Return the sample rate currently used by the instance */
	virtual int getSampleRate() = 0;

	/**
	 * Global init, calls the following methods:
	 * - static class 'classInit': static tables initialization
	 * - 'instanceInit': constants and instance state initialization
	 *
	 * @param sample_rate - the sampling rate in Hz
	 */
	virtual void init(int sample_rate) = 0;

	/**
	 * Init instance state
	 *
	 * @param sample_rate - the sampling rate in Hz
	 */
	virtual void instanceInit(int sample_rate) = 0;

	/**
	 * Init instance constant state
	 *
	 * @param sample_rate - the sampling rate in HZ
	 */
	virtual void instanceConstants(int sample_rate) = 0;

	/* Init default control parameters values */
	virtual void instanceResetUserInterface() = 0;

	/* Init instance state (like delay lines..) but keep the control parameter values */
	virtual void instanceClear() = 0;

	/**
	 * Return a clone of the instance.
	 *
	 * @return a copy of the instance on success, otherwise a null pointer.
	 */
	virtual FaustBase* clone() = 0;

	/**
	 * Trigger the Meta* parameter with instance specific calls to 'declare'
	 * (key, value) metadata.
	 *
	 * @param m - the Meta* meta user
	 */
	virtual void metadata(Meta* m) = 0;

	/**
	 * DSP instance computation, to be called with successive in/out audio buffers.
	 *
	 * @param count - the number of frames to compute
	 * @param inputs - the input audio buffers as an array of non-interleaved
	 * FAUSTFLOAT samples (eiher float, double or quad)
	 * @param outputs - the output audio buffers as an array of non-interleaved
	 * FAUSTFLOAT samples (eiher float, double or quad)
	 *
	 */
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
};

} } } // vae::core::faust



#endif // _VAE_FAUST_COMMON
