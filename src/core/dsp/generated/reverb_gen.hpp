/* ------------------------------------------------------------
name: "reverb"
Code generated with Faust 2.54.9 (https://faust.grame.fr)
Compilation options: -a /home/usr/git/audio/VAEG/VAE/src/core/dsp/vae_faust_arch.hpp -lang cpp -i -cm -mapp -cn Reverb -scn FaustBase -es 1 -exp10 -mcd 16 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __Reverb_H__
#define  __Reverb_H__


#include "../vae_faust_common.hpp"
// We prevent namespace problems by including theses before the namsapces
// below. the includes inside from faust shouldn't cause any trouble then.
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

namespace vae { namespace core { namespace effect {
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Reverb
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float Reverb_faustpower2_f(float value) {
	return value * value;
}

class Reverb final : public FaustBase {
	
 private:
	
	FAUSTFLOAT fVslider0;
	float fRec1_perm[4];
	float fRec0_perm[4];
	FAUSTFLOAT fVslider1;
	float fRec14_perm[4];
	float fRec13_perm[4];
	int fSampleRate;
	float fConst2;
	float fConst3;
	float fRec15_perm[4];
	FAUSTFLOAT fVslider2;
	float fRec17_perm[4];
	float fRec16_perm[4];
	float fConst7;
	float fConst8;
	float fConst9;
	float fRec12_perm[4];
	FAUSTFLOAT fVslider3;
	float fRec19_perm[4];
	float fRec18_perm[4];
	float fYec0[16384];
	int fYec0_idx;
	int fYec0_idx_save;
	float fConst10;
	float fYec1[65536];
	int fYec1_idx;
	int fYec1_idx_save;
	int iConst17;
	float fYec2[4096];
	int fYec2_idx;
	int fYec2_idx_save;
	int iConst20;
	float fRec10_perm[4];
	float fRec23_perm[4];
	float fConst24;
	float fRec22_perm[4];
	float fYec3[65536];
	int fYec3_idx;
	int fYec3_idx_save;
	int iConst31;
	float fYec4[8192];
	int fYec4_idx;
	int fYec4_idx_save;
	int iConst34;
	float fRec20_perm[4];
	float fRec27_perm[4];
	float fConst38;
	float fRec26_perm[4];
	float fYec5[65536];
	int fYec5_idx;
	int fYec5_idx_save;
	int iConst45;
	float fYec6[8192];
	int fYec6_idx;
	int fYec6_idx_save;
	int iConst48;
	float fRec24_perm[4];
	float fRec31_perm[4];
	float fConst52;
	float fRec30_perm[4];
	float fYec7[65536];
	int fYec7_idx;
	int fYec7_idx_save;
	int iConst59;
	float fYec8[8192];
	int fYec8_idx;
	int fYec8_idx_save;
	int iConst62;
	float fRec28_perm[4];
	float fRec35_perm[4];
	float fConst66;
	float fRec34_perm[4];
	float fYec9[16384];
	int fYec9_idx;
	int fYec9_idx_save;
	float fYec10[32768];
	int fYec10_idx;
	int fYec10_idx_save;
	int iConst73;
	float fYec11[4096];
	int fYec11_idx;
	int fYec11_idx_save;
	int iConst76;
	float fRec32_perm[4];
	float fRec39_perm[4];
	float fConst80;
	float fRec38_perm[4];
	float fYec12[32768];
	int fYec12_idx;
	int fYec12_idx_save;
	int iConst87;
	float fYec13[8192];
	int fYec13_idx;
	int fYec13_idx_save;
	int iConst90;
	float fRec36_perm[4];
	float fRec43_perm[4];
	float fConst94;
	float fRec42_perm[4];
	float fYec14[65536];
	int fYec14_idx;
	int fYec14_idx_save;
	int iConst101;
	float fYec15[8192];
	int fYec15_idx;
	int fYec15_idx_save;
	int iConst104;
	float fRec40_perm[4];
	float fRec47_perm[4];
	float fConst108;
	float fRec46_perm[4];
	float fYec16[32768];
	int fYec16_idx;
	int fYec16_idx_save;
	int iConst115;
	float fYec17[4096];
	int fYec17_idx;
	int fYec17_idx_save;
	int iConst118;
	float fRec44_perm[4];
	float fRec2_perm[4];
	float fRec3_perm[4];
	float fRec4_perm[4];
	float fRec5_perm[4];
	float fRec6_perm[4];
	float fRec7_perm[4];
	float fRec8_perm[4];
	float fRec9_perm[4];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.9");
		m->declare("compile_options", "-a /home/usr/git/audio/VAEG/VAE/src/core/dsp/vae_faust_arch.hpp -lang cpp -i -cm -mapp -cn Reverb -scn FaustBase -es 1 -exp10 -mcd 16 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "reverb.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "reverb");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.3");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
	}

	static constexpr int getStaticNumInputs() {
		return 2;
	}
	static constexpr int getStaticNumOutputs() {
		return 2;
	}
	int getNumInputs() {
		return 2;
	}
	int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = 1.0f / std::tan(1570.7964f / fConst0);
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / (fConst1 + 1.0f);
		float fConst4 = 0.219991f * fConst0 + 0.5f;
		float fConst5 = float(int(fConst4));
		float fConst6 = ((fConst4 == fConst5) ? fConst4 : ((fConst4 >= 0.0f) ? fConst5 : fConst5 + -1.0f));
		fConst7 = 0.0f - 6.9077554f * fConst6;
		fConst8 = 0.1f / fConst0;
		fConst9 = std::cos(18849.557f / fConst0);
		fConst10 = 0.5f * fConst0;
		float fConst11 = 0.019123f * fConst0 + 0.5f;
		float fConst12 = float(int(fConst11));
		float fConst13 = ((fConst11 == fConst12) ? fConst11 : ((fConst11 >= 0.0f) ? fConst12 : fConst12 + -1.0f));
		float fConst14 = fConst6 - fConst13;
		float fConst15 = 1.442695f * std::log(fConst14);
		float fConst16 = float(int(fConst15));
		iConst17 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst15 == fConst16) ? fConst15 : ((fConst15 >= 0.0f) ? fConst16 + 1.0f : fConst16)))))), std::max<float>(0.0f, fConst14)));
		float fConst18 = 1.442695f * std::log(fConst13);
		float fConst19 = float(int(fConst18));
		iConst20 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst18 == fConst19) ? fConst18 : ((fConst18 >= 0.0f) ? fConst19 + 1.0f : fConst19)))))), std::max<float>(0.0f, fConst13 + -1.0f)));
		float fConst21 = 0.256891f * fConst0 + 0.5f;
		float fConst22 = float(int(fConst21));
		float fConst23 = ((fConst21 == fConst22) ? fConst21 : ((fConst21 >= 0.0f) ? fConst22 : fConst22 + -1.0f));
		fConst24 = 0.0f - 6.9077554f * fConst23;
		float fConst25 = 0.027333f * fConst0 + 0.5f;
		float fConst26 = float(int(fConst25));
		float fConst27 = ((fConst25 == fConst26) ? fConst25 : ((fConst25 >= 0.0f) ? fConst26 : fConst26 + -1.0f));
		float fConst28 = fConst23 - fConst27;
		float fConst29 = 1.442695f * std::log(fConst28);
		float fConst30 = float(int(fConst29));
		iConst31 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst29 == fConst30) ? fConst29 : ((fConst29 >= 0.0f) ? fConst30 + 1.0f : fConst30)))))), std::max<float>(0.0f, fConst28)));
		float fConst32 = 1.442695f * std::log(fConst27);
		float fConst33 = float(int(fConst32));
		iConst34 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst32 == fConst33) ? fConst32 : ((fConst32 >= 0.0f) ? fConst33 + 1.0f : fConst33)))))), std::max<float>(0.0f, fConst27 + -1.0f)));
		float fConst35 = 0.192303f * fConst0 + 0.5f;
		float fConst36 = float(int(fConst35));
		float fConst37 = ((fConst35 == fConst36) ? fConst35 : ((fConst35 >= 0.0f) ? fConst36 : fConst36 + -1.0f));
		fConst38 = 0.0f - 6.9077554f * fConst37;
		float fConst39 = 0.029291f * fConst0 + 0.5f;
		float fConst40 = float(int(fConst39));
		float fConst41 = ((fConst39 == fConst40) ? fConst39 : ((fConst39 >= 0.0f) ? fConst40 : fConst40 + -1.0f));
		float fConst42 = fConst37 - fConst41;
		float fConst43 = 1.442695f * std::log(fConst42);
		float fConst44 = float(int(fConst43));
		iConst45 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst43 == fConst44) ? fConst43 : ((fConst43 >= 0.0f) ? fConst44 + 1.0f : fConst44)))))), std::max<float>(0.0f, fConst42)));
		float fConst46 = 1.442695f * std::log(fConst41);
		float fConst47 = float(int(fConst46));
		iConst48 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst46 == fConst47) ? fConst46 : ((fConst46 >= 0.0f) ? fConst47 + 1.0f : fConst47)))))), std::max<float>(0.0f, fConst41 + -1.0f)));
		float fConst49 = 0.210389f * fConst0 + 0.5f;
		float fConst50 = float(int(fConst49));
		float fConst51 = ((fConst49 == fConst50) ? fConst49 : ((fConst49 >= 0.0f) ? fConst50 : fConst50 + -1.0f));
		fConst52 = 0.0f - 6.9077554f * fConst51;
		float fConst53 = 0.024421f * fConst0 + 0.5f;
		float fConst54 = float(int(fConst53));
		float fConst55 = ((fConst53 == fConst54) ? fConst53 : ((fConst53 >= 0.0f) ? fConst54 : fConst54 + -1.0f));
		float fConst56 = fConst51 - fConst55;
		float fConst57 = 1.442695f * std::log(fConst56);
		float fConst58 = float(int(fConst57));
		iConst59 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst57 == fConst58) ? fConst57 : ((fConst57 >= 0.0f) ? fConst58 + 1.0f : fConst58)))))), std::max<float>(0.0f, fConst56)));
		float fConst60 = 1.442695f * std::log(fConst55);
		float fConst61 = float(int(fConst60));
		iConst62 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst60 == fConst61) ? fConst60 : ((fConst60 >= 0.0f) ? fConst61 + 1.0f : fConst61)))))), std::max<float>(0.0f, fConst55 + -1.0f)));
		float fConst63 = 0.125f * fConst0 + 0.5f;
		float fConst64 = float(int(fConst63));
		float fConst65 = ((fConst63 == fConst64) ? fConst63 : ((fConst63 >= 0.0f) ? fConst64 : fConst64 + -1.0f));
		fConst66 = 0.0f - 6.9077554f * fConst65;
		float fConst67 = 0.013458f * fConst0 + 0.5f;
		float fConst68 = float(int(fConst67));
		float fConst69 = ((fConst67 == fConst68) ? fConst67 : ((fConst67 >= 0.0f) ? fConst68 : fConst68 + -1.0f));
		float fConst70 = fConst65 - fConst69;
		float fConst71 = 1.442695f * std::log(fConst70);
		float fConst72 = float(int(fConst71));
		iConst73 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst71 == fConst72) ? fConst71 : ((fConst71 >= 0.0f) ? fConst72 + 1.0f : fConst72)))))), std::max<float>(0.0f, fConst70)));
		float fConst74 = 1.442695f * std::log(fConst69);
		float fConst75 = float(int(fConst74));
		iConst76 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst74 == fConst75) ? fConst74 : ((fConst74 >= 0.0f) ? fConst75 + 1.0f : fConst75)))))), std::max<float>(0.0f, fConst69 + -1.0f)));
		float fConst77 = 0.127837f * fConst0 + 0.5f;
		float fConst78 = float(int(fConst77));
		float fConst79 = ((fConst77 == fConst78) ? fConst77 : ((fConst77 >= 0.0f) ? fConst78 : fConst78 + -1.0f));
		fConst80 = 0.0f - 6.9077554f * fConst79;
		float fConst81 = 0.031604f * fConst0 + 0.5f;
		float fConst82 = float(int(fConst81));
		float fConst83 = ((fConst81 == fConst82) ? fConst81 : ((fConst81 >= 0.0f) ? fConst82 : fConst82 + -1.0f));
		float fConst84 = fConst79 - fConst83;
		float fConst85 = 1.442695f * std::log(fConst84);
		float fConst86 = float(int(fConst85));
		iConst87 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst85 == fConst86) ? fConst85 : ((fConst85 >= 0.0f) ? fConst86 + 1.0f : fConst86)))))), std::max<float>(0.0f, fConst84)));
		float fConst88 = 1.442695f * std::log(fConst83);
		float fConst89 = float(int(fConst88));
		iConst90 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst88 == fConst89) ? fConst88 : ((fConst88 >= 0.0f) ? fConst89 + 1.0f : fConst89)))))), std::max<float>(0.0f, fConst83 + -1.0f)));
		float fConst91 = 0.174713f * fConst0 + 0.5f;
		float fConst92 = float(int(fConst91));
		float fConst93 = ((fConst91 == fConst92) ? fConst91 : ((fConst91 >= 0.0f) ? fConst92 : fConst92 + -1.0f));
		fConst94 = 0.0f - 6.9077554f * fConst93;
		float fConst95 = 0.022904f * fConst0 + 0.5f;
		float fConst96 = float(int(fConst95));
		float fConst97 = ((fConst95 == fConst96) ? fConst95 : ((fConst95 >= 0.0f) ? fConst96 : fConst96 + -1.0f));
		float fConst98 = fConst93 - fConst97;
		float fConst99 = 1.442695f * std::log(fConst98);
		float fConst100 = float(int(fConst99));
		iConst101 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst99 == fConst100) ? fConst99 : ((fConst99 >= 0.0f) ? fConst100 + 1.0f : fConst100)))))), std::max<float>(0.0f, fConst98)));
		float fConst102 = 1.442695f * std::log(fConst97);
		float fConst103 = float(int(fConst102));
		iConst104 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst102 == fConst103) ? fConst102 : ((fConst102 >= 0.0f) ? fConst103 + 1.0f : fConst103)))))), std::max<float>(0.0f, fConst97 + -1.0f)));
		float fConst105 = 0.153129f * fConst0 + 0.5f;
		float fConst106 = float(int(fConst105));
		float fConst107 = ((fConst105 == fConst106) ? fConst105 : ((fConst105 >= 0.0f) ? fConst106 : fConst106 + -1.0f));
		fConst108 = 0.0f - 6.9077554f * fConst107;
		float fConst109 = 0.020346f * fConst0 + 0.5f;
		float fConst110 = float(int(fConst109));
		float fConst111 = ((fConst109 == fConst110) ? fConst109 : ((fConst109 >= 0.0f) ? fConst110 : fConst110 + -1.0f));
		float fConst112 = fConst107 - fConst111;
		float fConst113 = 1.442695f * std::log(fConst112);
		float fConst114 = float(int(fConst113));
		iConst115 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst113 == fConst114) ? fConst113 : ((fConst113 >= 0.0f) ? fConst114 + 1.0f : fConst114)))))), std::max<float>(0.0f, fConst112)));
		float fConst116 = 1.442695f * std::log(fConst111);
		float fConst117 = float(int(fConst116));
		iConst118 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst116 == fConst117) ? fConst116 : ((fConst116 >= 0.0f) ? fConst117 + 1.0f : fConst117)))))), std::max<float>(0.0f, fConst111 + -1.0f)));
	}
	
	void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(1.0f);
		fVslider1 = FAUSTFLOAT(0.5f);
		fVslider2 = FAUSTFLOAT(0.5f);
		fVslider3 = FAUSTFLOAT(0.5f);
	}
	
	void instanceClear() {
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			fRec1_perm[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec0_perm[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec14_perm[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			fRec13_perm[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			fRec15_perm[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec17_perm[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec16_perm[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec12_perm[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec19_perm[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec18_perm[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 16384; l10 = l10 + 1) {
			fYec0[l10] = 0.0f;
		}
		fYec0_idx = 0;
		fYec0_idx_save = 0;
		for (int l11 = 0; l11 < 65536; l11 = l11 + 1) {
			fYec1[l11] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
		for (int l12 = 0; l12 < 4096; l12 = l12 + 1) {
			fYec2[l12] = 0.0f;
		}
		fYec2_idx = 0;
		fYec2_idx_save = 0;
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec10_perm[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec23_perm[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec22_perm[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 65536; l16 = l16 + 1) {
			fYec3[l16] = 0.0f;
		}
		fYec3_idx = 0;
		fYec3_idx_save = 0;
		for (int l17 = 0; l17 < 8192; l17 = l17 + 1) {
			fYec4[l17] = 0.0f;
		}
		fYec4_idx = 0;
		fYec4_idx_save = 0;
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec20_perm[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 4; l19 = l19 + 1) {
			fRec27_perm[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 4; l20 = l20 + 1) {
			fRec26_perm[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 65536; l21 = l21 + 1) {
			fYec5[l21] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		for (int l22 = 0; l22 < 8192; l22 = l22 + 1) {
			fYec6[l22] = 0.0f;
		}
		fYec6_idx = 0;
		fYec6_idx_save = 0;
		for (int l23 = 0; l23 < 4; l23 = l23 + 1) {
			fRec24_perm[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 4; l24 = l24 + 1) {
			fRec31_perm[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 4; l25 = l25 + 1) {
			fRec30_perm[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 65536; l26 = l26 + 1) {
			fYec7[l26] = 0.0f;
		}
		fYec7_idx = 0;
		fYec7_idx_save = 0;
		for (int l27 = 0; l27 < 8192; l27 = l27 + 1) {
			fYec8[l27] = 0.0f;
		}
		fYec8_idx = 0;
		fYec8_idx_save = 0;
		for (int l28 = 0; l28 < 4; l28 = l28 + 1) {
			fRec28_perm[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 4; l29 = l29 + 1) {
			fRec35_perm[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 4; l30 = l30 + 1) {
			fRec34_perm[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 16384; l31 = l31 + 1) {
			fYec9[l31] = 0.0f;
		}
		fYec9_idx = 0;
		fYec9_idx_save = 0;
		for (int l32 = 0; l32 < 32768; l32 = l32 + 1) {
			fYec10[l32] = 0.0f;
		}
		fYec10_idx = 0;
		fYec10_idx_save = 0;
		for (int l33 = 0; l33 < 4096; l33 = l33 + 1) {
			fYec11[l33] = 0.0f;
		}
		fYec11_idx = 0;
		fYec11_idx_save = 0;
		for (int l34 = 0; l34 < 4; l34 = l34 + 1) {
			fRec32_perm[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 4; l35 = l35 + 1) {
			fRec39_perm[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 4; l36 = l36 + 1) {
			fRec38_perm[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 32768; l37 = l37 + 1) {
			fYec12[l37] = 0.0f;
		}
		fYec12_idx = 0;
		fYec12_idx_save = 0;
		for (int l38 = 0; l38 < 8192; l38 = l38 + 1) {
			fYec13[l38] = 0.0f;
		}
		fYec13_idx = 0;
		fYec13_idx_save = 0;
		for (int l39 = 0; l39 < 4; l39 = l39 + 1) {
			fRec36_perm[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 4; l40 = l40 + 1) {
			fRec43_perm[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 4; l41 = l41 + 1) {
			fRec42_perm[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 65536; l42 = l42 + 1) {
			fYec14[l42] = 0.0f;
		}
		fYec14_idx = 0;
		fYec14_idx_save = 0;
		for (int l43 = 0; l43 < 8192; l43 = l43 + 1) {
			fYec15[l43] = 0.0f;
		}
		fYec15_idx = 0;
		fYec15_idx_save = 0;
		for (int l44 = 0; l44 < 4; l44 = l44 + 1) {
			fRec40_perm[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 4; l45 = l45 + 1) {
			fRec47_perm[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 4; l46 = l46 + 1) {
			fRec46_perm[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 32768; l47 = l47 + 1) {
			fYec16[l47] = 0.0f;
		}
		fYec16_idx = 0;
		fYec16_idx_save = 0;
		for (int l48 = 0; l48 < 4096; l48 = l48 + 1) {
			fYec17[l48] = 0.0f;
		}
		fYec17_idx = 0;
		fYec17_idx_save = 0;
		for (int l49 = 0; l49 < 4; l49 = l49 + 1) {
			fRec44_perm[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 4; l50 = l50 + 1) {
			fRec2_perm[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 4; l51 = l51 + 1) {
			fRec3_perm[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 4; l52 = l52 + 1) {
			fRec4_perm[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 4; l53 = l53 + 1) {
			fRec5_perm[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 4; l54 = l54 + 1) {
			fRec6_perm[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 4; l55 = l55 + 1) {
			fRec7_perm[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 4; l56 = l56 + 1) {
			fRec8_perm[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 4; l57 = l57 + 1) {
			fRec9_perm[l57] = 0.0f;
		}
	}
	
	void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	Reverb* clone() {
		return new Reverb();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("reverb");
		ui_interface->addVerticalSlider("fb1", &fVslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("fb2", &fVslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("mix", &fVslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("pre", &fVslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fSlow0 = 0.001f * float(fVslider0);
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fRec0_tmp[36];
		float* fRec0 = &fRec0_tmp[4];
		float fSlow1 = 0.001f * float(fVslider1);
		float fRec14_tmp[36];
		float* fRec14 = &fRec14_tmp[4];
		float fRec13_tmp[36];
		float* fRec13 = &fRec13_tmp[4];
		float fRec15_tmp[36];
		float* fRec15 = &fRec15_tmp[4];
		float fSlow2 = 0.001f * float(fVslider2);
		float fRec17_tmp[36];
		float* fRec17 = &fRec17_tmp[4];
		float fRec16_tmp[36];
		float* fRec16 = &fRec16_tmp[4];
		float fZec0[32];
		float fZec1[32];
		float fZec2[32];
		float fZec3[32];
		float fZec4[32];
		float fZec5[32];
		float fZec6[32];
		float fZec7[32];
		float fRec12_tmp[36];
		float* fRec12 = &fRec12_tmp[4];
		float fSlow3 = 0.001f * float(fVslider3);
		float fRec19_tmp[36];
		float* fRec19 = &fRec19_tmp[4];
		float fRec18_tmp[36];
		float* fRec18 = &fRec18_tmp[4];
		int iZec8[32];
		float fZec9[32];
		float fRec10_tmp[36];
		float* fRec10 = &fRec10_tmp[4];
		float fRec11[32];
		float fRec23_tmp[36];
		float* fRec23 = &fRec23_tmp[4];
		float fZec10[32];
		float fZec11[32];
		float fZec12[32];
		float fZec13[32];
		float fZec14[32];
		float fZec15[32];
		float fRec22_tmp[36];
		float* fRec22 = &fRec22_tmp[4];
		float fRec20_tmp[36];
		float* fRec20 = &fRec20_tmp[4];
		float fRec21[32];
		float fRec27_tmp[36];
		float* fRec27 = &fRec27_tmp[4];
		float fZec16[32];
		float fZec17[32];
		float fZec18[32];
		float fZec19[32];
		float fZec20[32];
		float fZec21[32];
		float fRec26_tmp[36];
		float* fRec26 = &fRec26_tmp[4];
		float fRec24_tmp[36];
		float* fRec24 = &fRec24_tmp[4];
		float fRec25[32];
		float fRec31_tmp[36];
		float* fRec31 = &fRec31_tmp[4];
		float fZec22[32];
		float fZec23[32];
		float fZec24[32];
		float fZec25[32];
		float fZec26[32];
		float fZec27[32];
		float fRec30_tmp[36];
		float* fRec30 = &fRec30_tmp[4];
		float fRec28_tmp[36];
		float* fRec28 = &fRec28_tmp[4];
		float fRec29[32];
		float fRec35_tmp[36];
		float* fRec35 = &fRec35_tmp[4];
		float fZec28[32];
		float fZec29[32];
		float fZec30[32];
		float fZec31[32];
		float fZec32[32];
		float fZec33[32];
		float fRec34_tmp[36];
		float* fRec34 = &fRec34_tmp[4];
		float fZec34[32];
		float fRec32_tmp[36];
		float* fRec32 = &fRec32_tmp[4];
		float fRec33[32];
		float fRec39_tmp[36];
		float* fRec39 = &fRec39_tmp[4];
		float fZec35[32];
		float fZec36[32];
		float fZec37[32];
		float fZec38[32];
		float fZec39[32];
		float fZec40[32];
		float fRec38_tmp[36];
		float* fRec38 = &fRec38_tmp[4];
		float fRec36_tmp[36];
		float* fRec36 = &fRec36_tmp[4];
		float fRec37[32];
		float fRec43_tmp[36];
		float* fRec43 = &fRec43_tmp[4];
		float fZec41[32];
		float fZec42[32];
		float fZec43[32];
		float fZec44[32];
		float fZec45[32];
		float fZec46[32];
		float fRec42_tmp[36];
		float* fRec42 = &fRec42_tmp[4];
		float fRec40_tmp[36];
		float* fRec40 = &fRec40_tmp[4];
		float fRec41[32];
		float fRec47_tmp[36];
		float* fRec47 = &fRec47_tmp[4];
		float fZec47[32];
		float fZec48[32];
		float fZec49[32];
		float fZec50[32];
		float fZec51[32];
		float fZec52[32];
		float fRec46_tmp[36];
		float* fRec46 = &fRec46_tmp[4];
		float fRec44_tmp[36];
		float* fRec44 = &fRec44_tmp[4];
		float fRec45[32];
		float fZec53[32];
		float fZec54[32];
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		float fZec55[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fRec5_tmp[36];
		float* fRec5 = &fRec5_tmp[4];
		float fZec56[32];
		float fZec57[32];
		float fRec6_tmp[36];
		float* fRec6 = &fRec6_tmp[4];
		float fRec7_tmp[36];
		float* fRec7 = &fRec7_tmp[4];
		float fZec58[32];
		float fZec59[32];
		float fRec8_tmp[36];
		float* fRec8 = &fRec8_tmp[4];
		float fRec9_tmp[36];
		float* fRec9 = &fRec9_tmp[4];
		float fZec60[32];
		float fZec61[32];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 32); vindex = vindex + 32) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 32;
			/* Recursive loop 0 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec14_tmp[j4] = fRec14_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[i] = fSlow1 + 0.999f * fRec14[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec14_perm[j5] = fRec14_tmp[vsize + j5];
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec13_tmp[j6] = fRec13_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = 0.001f * fRec14[i] + 0.999f * fRec13[i - 1];
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec13_perm[j7] = fRec13_tmp[vsize + j7];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::min<float>(std::max<float>(fRec13[i], 0.02f), 0.97f);
			}
			/* Recursive loop 3 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec19_tmp[j16] = fRec19_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = fSlow3 + 0.999f * fRec19[i - 1];
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec19_perm[j17] = fRec19_tmp[vsize + j17];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = std::exp(fConst8 * (fConst7 / fZec0[i]));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = std::exp(fConst8 * (fConst24 / fZec0[i]));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = std::exp(fConst8 * (fConst38 / fZec0[i]));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = std::exp(fConst8 * (fConst52 / fZec0[i]));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = std::exp(fConst8 * (fConst66 / fZec0[i]));
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = std::exp(fConst8 * (fConst80 / fZec0[i]));
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec41[i] = std::exp(fConst8 * (fConst94 / fZec0[i]));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = std::exp(fConst8 * (fConst108 / fZec0[i]));
			}
			/* Recursive loop 12 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fRec1_tmp[j0] = fRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = fSlow0 + 0.999f * fRec1[i - 1];
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fRec1_perm[j1] = fRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = Reverb_faustpower2_f(fZec1[i]);
			}
			/* Recursive loop 14 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec17_tmp[j10] = fRec17_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = fSlow2 + 0.999f * fRec17[i - 1];
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec17_perm[j11] = fRec17_tmp[vsize + j11];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec18_tmp[j18] = fRec18_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = 0.001f * fRec19[i] + 0.999f * fRec18[i - 1];
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec18_perm[j19] = fRec18_tmp[vsize + j19];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = Reverb_faustpower2_f(fZec10[i]);
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = Reverb_faustpower2_f(fZec16[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = Reverb_faustpower2_f(fZec22[i]);
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = Reverb_faustpower2_f(fZec28[i]);
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = Reverb_faustpower2_f(fZec35[i]);
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec42[i] = Reverb_faustpower2_f(fZec41[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec48[i] = Reverb_faustpower2_f(fZec47[i]);
			}
			/* Recursive loop 23 */
			/* Pre code */
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = 0.001f * fRec1[i] + 0.999f * fRec0[i - 1];
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1.0f - fZec2[i];
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = 1.0f - fConst9 * fZec2[i];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec16_tmp[j12] = fRec16_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = 0.001f * fRec17[i] + 0.999f * fRec16[i - 1];
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec16_perm[j13] = fRec16_tmp[vsize + j13];
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst10 * std::min<float>(std::max<float>(fRec18[i], 0.01f), 0.97f))));
			}
			/* Vectorizable loop 28 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 16383;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 16383] = float(input1[i]);
			}
			/* Post code */
			fYec0_idx_save = vsize;
			/* Vectorizable loop 29 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = 1.0f - fConst9 * fZec11[i];
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = 1.0f - fZec11[i];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = 1.0f - fConst9 * fZec17[i];
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = 1.0f - fZec17[i];
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 1.0f - fConst9 * fZec23[i];
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = 1.0f - fZec23[i];
			}
			/* Vectorizable loop 35 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = 1.0f - fZec29[i];
			}
			/* Vectorizable loop 36 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = 1.0f - fConst9 * fZec29[i];
			}
			/* Vectorizable loop 37 */
			/* Pre code */
			fYec9_idx = (fYec9_idx + fYec9_idx_save) & 16383;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec9[(i + fYec9_idx) & 16383] = float(input0[i]);
			}
			/* Post code */
			fYec9_idx_save = vsize;
			/* Vectorizable loop 38 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = 1.0f - fZec36[i];
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = 1.0f - fConst9 * fZec36[i];
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec44[i] = 1.0f - fConst9 * fZec42[i];
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec43[i] = 1.0f - fZec42[i];
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec50[i] = 1.0f - fConst9 * fZec48[i];
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = 1.0f - fZec48[i];
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = std::min<float>(std::max<float>(fRec16[i], 0.02f), 0.97f);
			}
			/* Vectorizable loop 45 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec4[i]) / Reverb_faustpower2_f(fZec3[i]) + -1.0f));
			}
			/* Vectorizable loop 46 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = fZec4[i] / fZec3[i];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = 0.3f * fYec0[(i + fYec0_idx - iZec8[i]) & 16383];
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = fZec13[i] / fZec12[i];
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec13[i]) / Reverb_faustpower2_f(fZec12[i]) + -1.0f));
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = fZec19[i] / fZec18[i];
			}
			/* Vectorizable loop 51 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec19[i]) / Reverb_faustpower2_f(fZec18[i]) + -1.0f));
			}
			/* Vectorizable loop 52 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = fZec25[i] / fZec24[i];
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec25[i]) / Reverb_faustpower2_f(fZec24[i]) + -1.0f));
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = fZec31[i] / fZec30[i];
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec31[i]) / Reverb_faustpower2_f(fZec30[i]) + -1.0f));
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = 0.3f * fYec9[(i + fYec9_idx - iZec8[i]) & 16383];
			}
			/* Vectorizable loop 57 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = fZec38[i] / fZec37[i];
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec38[i]) / Reverb_faustpower2_f(fZec37[i]) + -1.0f));
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec46[i] = fZec44[i] / fZec43[i];
			}
			/* Vectorizable loop 60 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec44[i]) / Reverb_faustpower2_f(fZec43[i]) + -1.0f));
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec52[i] = fZec50[i] / fZec49[i];
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec51[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec50[i]) / Reverb_faustpower2_f(fZec49[i]) + -1.0f));
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec60[i] = std::min<float>(std::max<float>(fRec0[i], 0.0f), 1.0f);
			}
			/* Recursive loop 64 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec15_tmp[j8] = fRec15_perm[j8];
			}
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec12_tmp[j14] = fRec12_perm[j14];
			}
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 65535;
			fYec2_idx = (fYec2_idx + fYec2_idx_save) & 4095;
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec10_tmp[j20] = fRec10_perm[j20];
			}
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec23_tmp[j22] = fRec23_perm[j22];
			}
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec22_tmp[j24] = fRec22_perm[j24];
			}
			fYec3_idx = (fYec3_idx + fYec3_idx_save) & 65535;
			fYec4_idx = (fYec4_idx + fYec4_idx_save) & 8191;
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec20_tmp[j26] = fRec20_perm[j26];
			}
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec27_tmp[j28] = fRec27_perm[j28];
			}
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec26_tmp[j30] = fRec26_perm[j30];
			}
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 65535;
			fYec6_idx = (fYec6_idx + fYec6_idx_save) & 8191;
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec24_tmp[j32] = fRec24_perm[j32];
			}
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec31_tmp[j34] = fRec31_perm[j34];
			}
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec30_tmp[j36] = fRec30_perm[j36];
			}
			fYec7_idx = (fYec7_idx + fYec7_idx_save) & 65535;
			fYec8_idx = (fYec8_idx + fYec8_idx_save) & 8191;
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec28_tmp[j38] = fRec28_perm[j38];
			}
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec35_tmp[j40] = fRec35_perm[j40];
			}
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec34_tmp[j42] = fRec34_perm[j42];
			}
			fYec10_idx = (fYec10_idx + fYec10_idx_save) & 32767;
			fYec11_idx = (fYec11_idx + fYec11_idx_save) & 4095;
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec32_tmp[j44] = fRec32_perm[j44];
			}
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec39_tmp[j46] = fRec39_perm[j46];
			}
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fRec38_tmp[j48] = fRec38_perm[j48];
			}
			fYec12_idx = (fYec12_idx + fYec12_idx_save) & 32767;
			fYec13_idx = (fYec13_idx + fYec13_idx_save) & 8191;
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec36_tmp[j50] = fRec36_perm[j50];
			}
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec43_tmp[j52] = fRec43_perm[j52];
			}
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec42_tmp[j54] = fRec42_perm[j54];
			}
			fYec14_idx = (fYec14_idx + fYec14_idx_save) & 65535;
			fYec15_idx = (fYec15_idx + fYec15_idx_save) & 8191;
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec40_tmp[j56] = fRec40_perm[j56];
			}
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec47_tmp[j58] = fRec47_perm[j58];
			}
			for (int j60 = 0; j60 < 4; j60 = j60 + 1) {
				fRec46_tmp[j60] = fRec46_perm[j60];
			}
			fYec16_idx = (fYec16_idx + fYec16_idx_save) & 32767;
			fYec17_idx = (fYec17_idx + fYec17_idx_save) & 4095;
			for (int j62 = 0; j62 < 4; j62 = j62 + 1) {
				fRec44_tmp[j62] = fRec44_perm[j62];
			}
			for (int j64 = 0; j64 < 4; j64 = j64 + 1) {
				fRec2_tmp[j64] = fRec2_perm[j64];
			}
			for (int j66 = 0; j66 < 4; j66 = j66 + 1) {
				fRec3_tmp[j66] = fRec3_perm[j66];
			}
			for (int j68 = 0; j68 < 4; j68 = j68 + 1) {
				fRec4_tmp[j68] = fRec4_perm[j68];
			}
			for (int j70 = 0; j70 < 4; j70 = j70 + 1) {
				fRec5_tmp[j70] = fRec5_perm[j70];
			}
			for (int j72 = 0; j72 < 4; j72 = j72 + 1) {
				fRec6_tmp[j72] = fRec6_perm[j72];
			}
			for (int j74 = 0; j74 < 4; j74 = j74 + 1) {
				fRec7_tmp[j74] = fRec7_perm[j74];
			}
			for (int j76 = 0; j76 < 4; j76 = j76 + 1) {
				fRec8_tmp[j76] = fRec8_perm[j76];
			}
			for (int j78 = 0; j78 < 4; j78 = j78 + 1) {
				fRec9_tmp[j78] = fRec9_perm[j78];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = 0.0f - fConst3 * (fConst2 * fRec15[i - 1] - (fRec9[i - 1] + fRec9[i - 2]));
				fRec12[i] = fZec1[i] * (fZec5[i] + (1.0f - fZec6[i])) * (fRec9[i - 1] + fRec15[i] * (std::exp(fConst8 * (fConst7 / fZec7[i])) / fZec1[i] + -1.0f)) + (fZec6[i] - fZec5[i]) * fRec12[i - 1];
				fYec1[(i + fYec1_idx) & 65535] = 0.35355338f * fRec12[i] + 1e-20f;
				fYec2[(i + fYec2_idx) & 4095] = 0.6f * fRec10[i - 1] + fYec1[(i + fYec1_idx - iConst17) & 65535] - fZec9[i];
				fRec10[i] = fYec2[(i + fYec2_idx - iConst20) & 4095];
				fRec11[i] = 0.0f - 0.6f * fYec2[(i + fYec2_idx) & 4095];
				fRec23[i] = 0.0f - fConst3 * (fConst2 * fRec23[i - 1] - (fRec5[i - 1] + fRec5[i - 2]));
				fRec22[i] = fZec10[i] * (fZec14[i] + (1.0f - fZec15[i])) * (fRec5[i - 1] + fRec23[i] * (std::exp(fConst8 * (fConst24 / fZec7[i])) / fZec10[i] + -1.0f)) + (fZec15[i] - fZec14[i]) * fRec22[i - 1];
				fYec3[(i + fYec3_idx) & 65535] = 0.35355338f * fRec22[i] + 1e-20f;
				fYec4[(i + fYec4_idx) & 8191] = 0.6f * fRec20[i - 1] + fYec3[(i + fYec3_idx - iConst31) & 65535] - fZec9[i];
				fRec20[i] = fYec4[(i + fYec4_idx - iConst34) & 8191];
				fRec21[i] = 0.0f - 0.6f * fYec4[(i + fYec4_idx) & 8191];
				fRec27[i] = 0.0f - fConst3 * (fConst2 * fRec27[i - 1] - (fRec7[i - 1] + fRec7[i - 2]));
				fRec26[i] = fZec16[i] * (fZec20[i] + (1.0f - fZec21[i])) * (fRec7[i - 1] + fRec27[i] * (std::exp(fConst8 * (fConst38 / fZec7[i])) / fZec16[i] + -1.0f)) + (fZec21[i] - fZec20[i]) * fRec26[i - 1];
				fYec5[(i + fYec5_idx) & 65535] = 0.35355338f * fRec26[i] + 1e-20f;
				fYec6[(i + fYec6_idx) & 8191] = fYec5[(i + fYec5_idx - iConst45) & 65535] + fZec9[i] + 0.6f * fRec24[i - 1];
				fRec24[i] = fYec6[(i + fYec6_idx - iConst48) & 8191];
				fRec25[i] = 0.0f - 0.6f * fYec6[(i + fYec6_idx) & 8191];
				fRec31[i] = 0.0f - fConst3 * (fConst2 * fRec31[i - 1] - (fRec3[i - 1] + fRec3[i - 2]));
				fRec30[i] = fZec22[i] * (fZec26[i] + (1.0f - fZec27[i])) * (fRec3[i - 1] + fRec31[i] * (std::exp(fConst8 * (fConst52 / fZec7[i])) / fZec22[i] + -1.0f)) + (fZec27[i] - fZec26[i]) * fRec30[i - 1];
				fYec7[(i + fYec7_idx) & 65535] = 0.35355338f * fRec30[i] + 1e-20f;
				fYec8[(i + fYec8_idx) & 8191] = fZec9[i] + 0.6f * fRec28[i - 1] + fYec7[(i + fYec7_idx - iConst59) & 65535];
				fRec28[i] = fYec8[(i + fYec8_idx - iConst62) & 8191];
				fRec29[i] = 0.0f - 0.6f * fYec8[(i + fYec8_idx) & 8191];
				fRec35[i] = 0.0f - fConst3 * (fConst2 * fRec35[i - 1] - (fRec8[i - 1] + fRec8[i - 2]));
				fRec34[i] = fZec28[i] * (fZec32[i] + (1.0f - fZec33[i])) * (fRec8[i - 1] + fRec35[i] * (std::exp(fConst8 * (fConst66 / fZec7[i])) / fZec28[i] + -1.0f)) + (fZec33[i] - fZec32[i]) * fRec34[i - 1];
				fYec10[(i + fYec10_idx) & 32767] = 0.35355338f * fRec34[i] + 1e-20f;
				fYec11[(i + fYec11_idx) & 4095] = fYec10[(i + fYec10_idx - iConst73) & 32767] - (fZec34[i] + 0.6f * fRec32[i - 1]);
				fRec32[i] = fYec11[(i + fYec11_idx - iConst76) & 4095];
				fRec33[i] = 0.6f * fYec11[(i + fYec11_idx) & 4095];
				fRec39[i] = 0.0f - fConst3 * (fConst2 * fRec39[i - 1] - (fRec4[i - 1] + fRec4[i - 2]));
				fRec38[i] = fZec35[i] * (fZec39[i] + (1.0f - fZec40[i])) * (fRec4[i - 1] + fRec39[i] * (std::exp(fConst8 * (fConst80 / fZec7[i])) / fZec35[i] + -1.0f)) + (fZec40[i] - fZec39[i]) * fRec38[i - 1];
				fYec12[(i + fYec12_idx) & 32767] = 0.35355338f * fRec38[i] + 1e-20f;
				fYec13[(i + fYec13_idx) & 8191] = fYec12[(i + fYec12_idx - iConst87) & 32767] - (fZec34[i] + 0.6f * fRec36[i - 1]);
				fRec36[i] = fYec13[(i + fYec13_idx - iConst90) & 8191];
				fRec37[i] = 0.6f * fYec13[(i + fYec13_idx) & 8191];
				fRec43[i] = 0.0f - fConst3 * (fConst2 * fRec43[i - 1] - (fRec6[i - 1] + fRec6[i - 2]));
				fRec42[i] = fZec41[i] * (fZec45[i] + (1.0f - fZec46[i])) * (fRec6[i - 1] + fRec43[i] * (std::exp(fConst8 * (fConst94 / fZec7[i])) / fZec41[i] + -1.0f)) + (fZec46[i] - fZec45[i]) * fRec42[i - 1];
				fYec14[(i + fYec14_idx) & 65535] = 0.35355338f * fRec42[i] + 1e-20f;
				fYec15[(i + fYec15_idx) & 8191] = fZec34[i] + fYec14[(i + fYec14_idx - iConst101) & 65535] - 0.6f * fRec40[i - 1];
				fRec40[i] = fYec15[(i + fYec15_idx - iConst104) & 8191];
				fRec41[i] = 0.6f * fYec15[(i + fYec15_idx) & 8191];
				fRec47[i] = 0.0f - fConst3 * (fConst2 * fRec47[i - 1] - (fRec2[i - 1] + fRec2[i - 2]));
				fRec46[i] = fZec47[i] * (fZec51[i] + (1.0f - fZec52[i])) * (fRec2[i - 1] + fRec47[i] * (std::exp(fConst8 * (fConst108 / fZec7[i])) / fZec47[i] + -1.0f)) + (fZec52[i] - fZec51[i]) * fRec46[i - 1];
				fYec16[(i + fYec16_idx) & 32767] = 0.35355338f * fRec46[i] + 1e-20f;
				fYec17[(i + fYec17_idx) & 4095] = fYec16[(i + fYec16_idx - iConst115) & 32767] + fZec34[i] - 0.6f * fRec44[i - 1];
				fRec44[i] = fYec17[(i + fYec17_idx - iConst118) & 4095];
				fRec45[i] = 0.6f * fYec17[(i + fYec17_idx) & 4095];
				fZec53[i] = fRec45[i] + fRec41[i];
				fZec54[i] = fRec33[i] + fRec37[i] + fZec53[i];
				fRec2[i] = fRec10[i - 1] + fRec20[i - 1] + fRec24[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fRec11[i] + fRec21[i] + fRec25[i] + fRec29[i] + fZec54[i];
				fRec3[i] = fRec32[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fZec54[i] - (fRec10[i - 1] + fRec20[i - 1] + fRec24[i - 1] + fRec28[i - 1] + fRec11[i] + fRec21[i] + fRec29[i] + fRec25[i]);
				fZec55[i] = fRec37[i] + fRec33[i];
				fRec4[i] = fRec24[i - 1] + fRec28[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fRec25[i] + fRec29[i] + fZec53[i] - (fRec10[i - 1] + fRec20[i - 1] + fRec32[i - 1] + fRec36[i - 1] + fRec11[i] + fRec21[i] + fZec55[i]);
				fRec5[i] = fRec10[i - 1] + fRec20[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fRec11[i] + fRec21[i] + fZec53[i] - (fRec24[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec36[i - 1] + fRec25[i] + fRec29[i] + fZec55[i]);
				fZec56[i] = fRec41[i] + fRec33[i];
				fZec57[i] = fRec45[i] + fRec37[i];
				fRec6[i] = fRec20[i - 1] + fRec28[i - 1] + fRec36[i - 1] + fRec44[i - 1] + fRec21[i] + fRec29[i] + fZec57[i] - (fRec10[i - 1] + fRec24[i - 1] + fRec32[i - 1] + fRec40[i - 1] + fRec11[i] + fRec25[i] + fZec56[i]);
				fRec7[i] = fRec10[i - 1] + fRec24[i - 1] + fRec36[i - 1] + fRec44[i - 1] + fRec11[i] + fRec25[i] + fZec57[i] - (fRec20[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec40[i - 1] + fRec21[i] + fRec29[i] + fZec56[i]);
				fZec58[i] = fRec41[i] + fRec37[i];
				fZec59[i] = fRec45[i] + fRec33[i];
				fRec8[i] = fRec10[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec44[i - 1] + fRec11[i] + fRec29[i] + fZec59[i] - (fRec20[i - 1] + fRec24[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec21[i] + fRec25[i] + fZec58[i]);
				fRec9[i] = fRec20[i - 1] + fRec24[i - 1] + fRec32[i - 1] + fRec44[i - 1] + fRec21[i] + fRec25[i] + fZec59[i] - (fRec10[i - 1] + fRec28[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec11[i] + fRec29[i] + fZec58[i]);
			}
			/* Post code */
			fYec16_idx_save = vsize;
			fYec17_idx_save = vsize;
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec47_perm[j59] = fRec47_tmp[vsize + j59];
			}
			for (int j61 = 0; j61 < 4; j61 = j61 + 1) {
				fRec46_perm[j61] = fRec46_tmp[vsize + j61];
			}
			for (int j63 = 0; j63 < 4; j63 = j63 + 1) {
				fRec44_perm[j63] = fRec44_tmp[vsize + j63];
			}
			fYec14_idx_save = vsize;
			fYec15_idx_save = vsize;
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec43_perm[j53] = fRec43_tmp[vsize + j53];
			}
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec42_perm[j55] = fRec42_tmp[vsize + j55];
			}
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec40_perm[j57] = fRec40_tmp[vsize + j57];
			}
			fYec12_idx_save = vsize;
			fYec13_idx_save = vsize;
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec39_perm[j47] = fRec39_tmp[vsize + j47];
			}
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fRec38_perm[j49] = fRec38_tmp[vsize + j49];
			}
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec36_perm[j51] = fRec36_tmp[vsize + j51];
			}
			fYec10_idx_save = vsize;
			fYec11_idx_save = vsize;
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec35_perm[j41] = fRec35_tmp[vsize + j41];
			}
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec34_perm[j43] = fRec34_tmp[vsize + j43];
			}
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec32_perm[j45] = fRec32_tmp[vsize + j45];
			}
			fYec7_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec31_perm[j35] = fRec31_tmp[vsize + j35];
			}
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec30_perm[j37] = fRec30_tmp[vsize + j37];
			}
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec28_perm[j39] = fRec28_tmp[vsize + j39];
			}
			fYec5_idx_save = vsize;
			fYec6_idx_save = vsize;
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec27_perm[j29] = fRec27_tmp[vsize + j29];
			}
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec26_perm[j31] = fRec26_tmp[vsize + j31];
			}
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec24_perm[j33] = fRec24_tmp[vsize + j33];
			}
			fYec3_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec23_perm[j23] = fRec23_tmp[vsize + j23];
			}
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec22_perm[j25] = fRec22_tmp[vsize + j25];
			}
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec20_perm[j27] = fRec20_tmp[vsize + j27];
			}
			fYec1_idx_save = vsize;
			fYec2_idx_save = vsize;
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec15_perm[j9] = fRec15_tmp[vsize + j9];
			}
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec12_perm[j15] = fRec12_tmp[vsize + j15];
			}
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec10_perm[j21] = fRec10_tmp[vsize + j21];
			}
			for (int j65 = 0; j65 < 4; j65 = j65 + 1) {
				fRec2_perm[j65] = fRec2_tmp[vsize + j65];
			}
			for (int j67 = 0; j67 < 4; j67 = j67 + 1) {
				fRec3_perm[j67] = fRec3_tmp[vsize + j67];
			}
			for (int j69 = 0; j69 < 4; j69 = j69 + 1) {
				fRec4_perm[j69] = fRec4_tmp[vsize + j69];
			}
			for (int j71 = 0; j71 < 4; j71 = j71 + 1) {
				fRec5_perm[j71] = fRec5_tmp[vsize + j71];
			}
			for (int j73 = 0; j73 < 4; j73 = j73 + 1) {
				fRec6_perm[j73] = fRec6_tmp[vsize + j73];
			}
			for (int j75 = 0; j75 < 4; j75 = j75 + 1) {
				fRec7_perm[j75] = fRec7_tmp[vsize + j75];
			}
			for (int j77 = 0; j77 < 4; j77 = j77 + 1) {
				fRec8_perm[j77] = fRec8_tmp[vsize + j77];
			}
			for (int j79 = 0; j79 < 4; j79 = j79 + 1) {
				fRec9_perm[j79] = fRec9_tmp[vsize + j79];
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec61[i] = 1.0f - fZec60[i];
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec61[i] + 0.37f * (fRec3[i] + fRec4[i]) * fZec60[i]) + output0[i];
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec61[i] + 0.37f * (fRec3[i] - fRec4[i]) * fZec60[i]) + output1[i];
			}
		}
		/* Remaining frames */
		if ((vindex < count)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = count - vindex;
			/* Recursive loop 0 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec14_tmp[j4] = fRec14_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[i] = fSlow1 + 0.999f * fRec14[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec14_perm[j5] = fRec14_tmp[vsize + j5];
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec13_tmp[j6] = fRec13_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = 0.001f * fRec14[i] + 0.999f * fRec13[i - 1];
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec13_perm[j7] = fRec13_tmp[vsize + j7];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::min<float>(std::max<float>(fRec13[i], 0.02f), 0.97f);
			}
			/* Recursive loop 3 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec19_tmp[j16] = fRec19_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = fSlow3 + 0.999f * fRec19[i - 1];
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec19_perm[j17] = fRec19_tmp[vsize + j17];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = std::exp(fConst8 * (fConst7 / fZec0[i]));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = std::exp(fConst8 * (fConst24 / fZec0[i]));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = std::exp(fConst8 * (fConst38 / fZec0[i]));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = std::exp(fConst8 * (fConst52 / fZec0[i]));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = std::exp(fConst8 * (fConst66 / fZec0[i]));
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = std::exp(fConst8 * (fConst80 / fZec0[i]));
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec41[i] = std::exp(fConst8 * (fConst94 / fZec0[i]));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = std::exp(fConst8 * (fConst108 / fZec0[i]));
			}
			/* Recursive loop 12 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fRec1_tmp[j0] = fRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = fSlow0 + 0.999f * fRec1[i - 1];
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fRec1_perm[j1] = fRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = Reverb_faustpower2_f(fZec1[i]);
			}
			/* Recursive loop 14 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec17_tmp[j10] = fRec17_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = fSlow2 + 0.999f * fRec17[i - 1];
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec17_perm[j11] = fRec17_tmp[vsize + j11];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec18_tmp[j18] = fRec18_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = 0.001f * fRec19[i] + 0.999f * fRec18[i - 1];
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec18_perm[j19] = fRec18_tmp[vsize + j19];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = Reverb_faustpower2_f(fZec10[i]);
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = Reverb_faustpower2_f(fZec16[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = Reverb_faustpower2_f(fZec22[i]);
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = Reverb_faustpower2_f(fZec28[i]);
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = Reverb_faustpower2_f(fZec35[i]);
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec42[i] = Reverb_faustpower2_f(fZec41[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec48[i] = Reverb_faustpower2_f(fZec47[i]);
			}
			/* Recursive loop 23 */
			/* Pre code */
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = 0.001f * fRec1[i] + 0.999f * fRec0[i - 1];
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1.0f - fZec2[i];
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = 1.0f - fConst9 * fZec2[i];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec16_tmp[j12] = fRec16_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = 0.001f * fRec17[i] + 0.999f * fRec16[i - 1];
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec16_perm[j13] = fRec16_tmp[vsize + j13];
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst10 * std::min<float>(std::max<float>(fRec18[i], 0.01f), 0.97f))));
			}
			/* Vectorizable loop 28 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 16383;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 16383] = float(input1[i]);
			}
			/* Post code */
			fYec0_idx_save = vsize;
			/* Vectorizable loop 29 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = 1.0f - fConst9 * fZec11[i];
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = 1.0f - fZec11[i];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = 1.0f - fConst9 * fZec17[i];
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = 1.0f - fZec17[i];
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 1.0f - fConst9 * fZec23[i];
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = 1.0f - fZec23[i];
			}
			/* Vectorizable loop 35 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = 1.0f - fZec29[i];
			}
			/* Vectorizable loop 36 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = 1.0f - fConst9 * fZec29[i];
			}
			/* Vectorizable loop 37 */
			/* Pre code */
			fYec9_idx = (fYec9_idx + fYec9_idx_save) & 16383;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec9[(i + fYec9_idx) & 16383] = float(input0[i]);
			}
			/* Post code */
			fYec9_idx_save = vsize;
			/* Vectorizable loop 38 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = 1.0f - fZec36[i];
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = 1.0f - fConst9 * fZec36[i];
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec44[i] = 1.0f - fConst9 * fZec42[i];
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec43[i] = 1.0f - fZec42[i];
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec50[i] = 1.0f - fConst9 * fZec48[i];
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = 1.0f - fZec48[i];
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = std::min<float>(std::max<float>(fRec16[i], 0.02f), 0.97f);
			}
			/* Vectorizable loop 45 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec4[i]) / Reverb_faustpower2_f(fZec3[i]) + -1.0f));
			}
			/* Vectorizable loop 46 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = fZec4[i] / fZec3[i];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = 0.3f * fYec0[(i + fYec0_idx - iZec8[i]) & 16383];
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = fZec13[i] / fZec12[i];
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec13[i]) / Reverb_faustpower2_f(fZec12[i]) + -1.0f));
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = fZec19[i] / fZec18[i];
			}
			/* Vectorizable loop 51 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec19[i]) / Reverb_faustpower2_f(fZec18[i]) + -1.0f));
			}
			/* Vectorizable loop 52 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = fZec25[i] / fZec24[i];
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec25[i]) / Reverb_faustpower2_f(fZec24[i]) + -1.0f));
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = fZec31[i] / fZec30[i];
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec31[i]) / Reverb_faustpower2_f(fZec30[i]) + -1.0f));
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = 0.3f * fYec9[(i + fYec9_idx - iZec8[i]) & 16383];
			}
			/* Vectorizable loop 57 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = fZec38[i] / fZec37[i];
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec38[i]) / Reverb_faustpower2_f(fZec37[i]) + -1.0f));
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec46[i] = fZec44[i] / fZec43[i];
			}
			/* Vectorizable loop 60 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec44[i]) / Reverb_faustpower2_f(fZec43[i]) + -1.0f));
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec52[i] = fZec50[i] / fZec49[i];
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec51[i] = std::sqrt(std::max<float>(0.0f, Reverb_faustpower2_f(fZec50[i]) / Reverb_faustpower2_f(fZec49[i]) + -1.0f));
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec60[i] = std::min<float>(std::max<float>(fRec0[i], 0.0f), 1.0f);
			}
			/* Recursive loop 64 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec15_tmp[j8] = fRec15_perm[j8];
			}
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec12_tmp[j14] = fRec12_perm[j14];
			}
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 65535;
			fYec2_idx = (fYec2_idx + fYec2_idx_save) & 4095;
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec10_tmp[j20] = fRec10_perm[j20];
			}
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec23_tmp[j22] = fRec23_perm[j22];
			}
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec22_tmp[j24] = fRec22_perm[j24];
			}
			fYec3_idx = (fYec3_idx + fYec3_idx_save) & 65535;
			fYec4_idx = (fYec4_idx + fYec4_idx_save) & 8191;
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec20_tmp[j26] = fRec20_perm[j26];
			}
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec27_tmp[j28] = fRec27_perm[j28];
			}
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec26_tmp[j30] = fRec26_perm[j30];
			}
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 65535;
			fYec6_idx = (fYec6_idx + fYec6_idx_save) & 8191;
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec24_tmp[j32] = fRec24_perm[j32];
			}
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec31_tmp[j34] = fRec31_perm[j34];
			}
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec30_tmp[j36] = fRec30_perm[j36];
			}
			fYec7_idx = (fYec7_idx + fYec7_idx_save) & 65535;
			fYec8_idx = (fYec8_idx + fYec8_idx_save) & 8191;
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec28_tmp[j38] = fRec28_perm[j38];
			}
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec35_tmp[j40] = fRec35_perm[j40];
			}
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec34_tmp[j42] = fRec34_perm[j42];
			}
			fYec10_idx = (fYec10_idx + fYec10_idx_save) & 32767;
			fYec11_idx = (fYec11_idx + fYec11_idx_save) & 4095;
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec32_tmp[j44] = fRec32_perm[j44];
			}
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec39_tmp[j46] = fRec39_perm[j46];
			}
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fRec38_tmp[j48] = fRec38_perm[j48];
			}
			fYec12_idx = (fYec12_idx + fYec12_idx_save) & 32767;
			fYec13_idx = (fYec13_idx + fYec13_idx_save) & 8191;
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec36_tmp[j50] = fRec36_perm[j50];
			}
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec43_tmp[j52] = fRec43_perm[j52];
			}
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec42_tmp[j54] = fRec42_perm[j54];
			}
			fYec14_idx = (fYec14_idx + fYec14_idx_save) & 65535;
			fYec15_idx = (fYec15_idx + fYec15_idx_save) & 8191;
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec40_tmp[j56] = fRec40_perm[j56];
			}
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec47_tmp[j58] = fRec47_perm[j58];
			}
			for (int j60 = 0; j60 < 4; j60 = j60 + 1) {
				fRec46_tmp[j60] = fRec46_perm[j60];
			}
			fYec16_idx = (fYec16_idx + fYec16_idx_save) & 32767;
			fYec17_idx = (fYec17_idx + fYec17_idx_save) & 4095;
			for (int j62 = 0; j62 < 4; j62 = j62 + 1) {
				fRec44_tmp[j62] = fRec44_perm[j62];
			}
			for (int j64 = 0; j64 < 4; j64 = j64 + 1) {
				fRec2_tmp[j64] = fRec2_perm[j64];
			}
			for (int j66 = 0; j66 < 4; j66 = j66 + 1) {
				fRec3_tmp[j66] = fRec3_perm[j66];
			}
			for (int j68 = 0; j68 < 4; j68 = j68 + 1) {
				fRec4_tmp[j68] = fRec4_perm[j68];
			}
			for (int j70 = 0; j70 < 4; j70 = j70 + 1) {
				fRec5_tmp[j70] = fRec5_perm[j70];
			}
			for (int j72 = 0; j72 < 4; j72 = j72 + 1) {
				fRec6_tmp[j72] = fRec6_perm[j72];
			}
			for (int j74 = 0; j74 < 4; j74 = j74 + 1) {
				fRec7_tmp[j74] = fRec7_perm[j74];
			}
			for (int j76 = 0; j76 < 4; j76 = j76 + 1) {
				fRec8_tmp[j76] = fRec8_perm[j76];
			}
			for (int j78 = 0; j78 < 4; j78 = j78 + 1) {
				fRec9_tmp[j78] = fRec9_perm[j78];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = 0.0f - fConst3 * (fConst2 * fRec15[i - 1] - (fRec9[i - 1] + fRec9[i - 2]));
				fRec12[i] = fZec1[i] * (fZec5[i] + (1.0f - fZec6[i])) * (fRec9[i - 1] + fRec15[i] * (std::exp(fConst8 * (fConst7 / fZec7[i])) / fZec1[i] + -1.0f)) + (fZec6[i] - fZec5[i]) * fRec12[i - 1];
				fYec1[(i + fYec1_idx) & 65535] = 0.35355338f * fRec12[i] + 1e-20f;
				fYec2[(i + fYec2_idx) & 4095] = 0.6f * fRec10[i - 1] + fYec1[(i + fYec1_idx - iConst17) & 65535] - fZec9[i];
				fRec10[i] = fYec2[(i + fYec2_idx - iConst20) & 4095];
				fRec11[i] = 0.0f - 0.6f * fYec2[(i + fYec2_idx) & 4095];
				fRec23[i] = 0.0f - fConst3 * (fConst2 * fRec23[i - 1] - (fRec5[i - 1] + fRec5[i - 2]));
				fRec22[i] = fZec10[i] * (fZec14[i] + (1.0f - fZec15[i])) * (fRec5[i - 1] + fRec23[i] * (std::exp(fConst8 * (fConst24 / fZec7[i])) / fZec10[i] + -1.0f)) + (fZec15[i] - fZec14[i]) * fRec22[i - 1];
				fYec3[(i + fYec3_idx) & 65535] = 0.35355338f * fRec22[i] + 1e-20f;
				fYec4[(i + fYec4_idx) & 8191] = 0.6f * fRec20[i - 1] + fYec3[(i + fYec3_idx - iConst31) & 65535] - fZec9[i];
				fRec20[i] = fYec4[(i + fYec4_idx - iConst34) & 8191];
				fRec21[i] = 0.0f - 0.6f * fYec4[(i + fYec4_idx) & 8191];
				fRec27[i] = 0.0f - fConst3 * (fConst2 * fRec27[i - 1] - (fRec7[i - 1] + fRec7[i - 2]));
				fRec26[i] = fZec16[i] * (fZec20[i] + (1.0f - fZec21[i])) * (fRec7[i - 1] + fRec27[i] * (std::exp(fConst8 * (fConst38 / fZec7[i])) / fZec16[i] + -1.0f)) + (fZec21[i] - fZec20[i]) * fRec26[i - 1];
				fYec5[(i + fYec5_idx) & 65535] = 0.35355338f * fRec26[i] + 1e-20f;
				fYec6[(i + fYec6_idx) & 8191] = fYec5[(i + fYec5_idx - iConst45) & 65535] + fZec9[i] + 0.6f * fRec24[i - 1];
				fRec24[i] = fYec6[(i + fYec6_idx - iConst48) & 8191];
				fRec25[i] = 0.0f - 0.6f * fYec6[(i + fYec6_idx) & 8191];
				fRec31[i] = 0.0f - fConst3 * (fConst2 * fRec31[i - 1] - (fRec3[i - 1] + fRec3[i - 2]));
				fRec30[i] = fZec22[i] * (fZec26[i] + (1.0f - fZec27[i])) * (fRec3[i - 1] + fRec31[i] * (std::exp(fConst8 * (fConst52 / fZec7[i])) / fZec22[i] + -1.0f)) + (fZec27[i] - fZec26[i]) * fRec30[i - 1];
				fYec7[(i + fYec7_idx) & 65535] = 0.35355338f * fRec30[i] + 1e-20f;
				fYec8[(i + fYec8_idx) & 8191] = fZec9[i] + 0.6f * fRec28[i - 1] + fYec7[(i + fYec7_idx - iConst59) & 65535];
				fRec28[i] = fYec8[(i + fYec8_idx - iConst62) & 8191];
				fRec29[i] = 0.0f - 0.6f * fYec8[(i + fYec8_idx) & 8191];
				fRec35[i] = 0.0f - fConst3 * (fConst2 * fRec35[i - 1] - (fRec8[i - 1] + fRec8[i - 2]));
				fRec34[i] = fZec28[i] * (fZec32[i] + (1.0f - fZec33[i])) * (fRec8[i - 1] + fRec35[i] * (std::exp(fConst8 * (fConst66 / fZec7[i])) / fZec28[i] + -1.0f)) + (fZec33[i] - fZec32[i]) * fRec34[i - 1];
				fYec10[(i + fYec10_idx) & 32767] = 0.35355338f * fRec34[i] + 1e-20f;
				fYec11[(i + fYec11_idx) & 4095] = fYec10[(i + fYec10_idx - iConst73) & 32767] - (fZec34[i] + 0.6f * fRec32[i - 1]);
				fRec32[i] = fYec11[(i + fYec11_idx - iConst76) & 4095];
				fRec33[i] = 0.6f * fYec11[(i + fYec11_idx) & 4095];
				fRec39[i] = 0.0f - fConst3 * (fConst2 * fRec39[i - 1] - (fRec4[i - 1] + fRec4[i - 2]));
				fRec38[i] = fZec35[i] * (fZec39[i] + (1.0f - fZec40[i])) * (fRec4[i - 1] + fRec39[i] * (std::exp(fConst8 * (fConst80 / fZec7[i])) / fZec35[i] + -1.0f)) + (fZec40[i] - fZec39[i]) * fRec38[i - 1];
				fYec12[(i + fYec12_idx) & 32767] = 0.35355338f * fRec38[i] + 1e-20f;
				fYec13[(i + fYec13_idx) & 8191] = fYec12[(i + fYec12_idx - iConst87) & 32767] - (fZec34[i] + 0.6f * fRec36[i - 1]);
				fRec36[i] = fYec13[(i + fYec13_idx - iConst90) & 8191];
				fRec37[i] = 0.6f * fYec13[(i + fYec13_idx) & 8191];
				fRec43[i] = 0.0f - fConst3 * (fConst2 * fRec43[i - 1] - (fRec6[i - 1] + fRec6[i - 2]));
				fRec42[i] = fZec41[i] * (fZec45[i] + (1.0f - fZec46[i])) * (fRec6[i - 1] + fRec43[i] * (std::exp(fConst8 * (fConst94 / fZec7[i])) / fZec41[i] + -1.0f)) + (fZec46[i] - fZec45[i]) * fRec42[i - 1];
				fYec14[(i + fYec14_idx) & 65535] = 0.35355338f * fRec42[i] + 1e-20f;
				fYec15[(i + fYec15_idx) & 8191] = fZec34[i] + fYec14[(i + fYec14_idx - iConst101) & 65535] - 0.6f * fRec40[i - 1];
				fRec40[i] = fYec15[(i + fYec15_idx - iConst104) & 8191];
				fRec41[i] = 0.6f * fYec15[(i + fYec15_idx) & 8191];
				fRec47[i] = 0.0f - fConst3 * (fConst2 * fRec47[i - 1] - (fRec2[i - 1] + fRec2[i - 2]));
				fRec46[i] = fZec47[i] * (fZec51[i] + (1.0f - fZec52[i])) * (fRec2[i - 1] + fRec47[i] * (std::exp(fConst8 * (fConst108 / fZec7[i])) / fZec47[i] + -1.0f)) + (fZec52[i] - fZec51[i]) * fRec46[i - 1];
				fYec16[(i + fYec16_idx) & 32767] = 0.35355338f * fRec46[i] + 1e-20f;
				fYec17[(i + fYec17_idx) & 4095] = fYec16[(i + fYec16_idx - iConst115) & 32767] + fZec34[i] - 0.6f * fRec44[i - 1];
				fRec44[i] = fYec17[(i + fYec17_idx - iConst118) & 4095];
				fRec45[i] = 0.6f * fYec17[(i + fYec17_idx) & 4095];
				fZec53[i] = fRec45[i] + fRec41[i];
				fZec54[i] = fRec33[i] + fRec37[i] + fZec53[i];
				fRec2[i] = fRec10[i - 1] + fRec20[i - 1] + fRec24[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fRec11[i] + fRec21[i] + fRec25[i] + fRec29[i] + fZec54[i];
				fRec3[i] = fRec32[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fZec54[i] - (fRec10[i - 1] + fRec20[i - 1] + fRec24[i - 1] + fRec28[i - 1] + fRec11[i] + fRec21[i] + fRec29[i] + fRec25[i]);
				fZec55[i] = fRec37[i] + fRec33[i];
				fRec4[i] = fRec24[i - 1] + fRec28[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fRec25[i] + fRec29[i] + fZec53[i] - (fRec10[i - 1] + fRec20[i - 1] + fRec32[i - 1] + fRec36[i - 1] + fRec11[i] + fRec21[i] + fZec55[i]);
				fRec5[i] = fRec10[i - 1] + fRec20[i - 1] + fRec40[i - 1] + fRec44[i - 1] + fRec11[i] + fRec21[i] + fZec53[i] - (fRec24[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec36[i - 1] + fRec25[i] + fRec29[i] + fZec55[i]);
				fZec56[i] = fRec41[i] + fRec33[i];
				fZec57[i] = fRec45[i] + fRec37[i];
				fRec6[i] = fRec20[i - 1] + fRec28[i - 1] + fRec36[i - 1] + fRec44[i - 1] + fRec21[i] + fRec29[i] + fZec57[i] - (fRec10[i - 1] + fRec24[i - 1] + fRec32[i - 1] + fRec40[i - 1] + fRec11[i] + fRec25[i] + fZec56[i]);
				fRec7[i] = fRec10[i - 1] + fRec24[i - 1] + fRec36[i - 1] + fRec44[i - 1] + fRec11[i] + fRec25[i] + fZec57[i] - (fRec20[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec40[i - 1] + fRec21[i] + fRec29[i] + fZec56[i]);
				fZec58[i] = fRec41[i] + fRec37[i];
				fZec59[i] = fRec45[i] + fRec33[i];
				fRec8[i] = fRec10[i - 1] + fRec28[i - 1] + fRec32[i - 1] + fRec44[i - 1] + fRec11[i] + fRec29[i] + fZec59[i] - (fRec20[i - 1] + fRec24[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec21[i] + fRec25[i] + fZec58[i]);
				fRec9[i] = fRec20[i - 1] + fRec24[i - 1] + fRec32[i - 1] + fRec44[i - 1] + fRec21[i] + fRec25[i] + fZec59[i] - (fRec10[i - 1] + fRec28[i - 1] + fRec36[i - 1] + fRec40[i - 1] + fRec11[i] + fRec29[i] + fZec58[i]);
			}
			/* Post code */
			fYec16_idx_save = vsize;
			fYec17_idx_save = vsize;
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec47_perm[j59] = fRec47_tmp[vsize + j59];
			}
			for (int j61 = 0; j61 < 4; j61 = j61 + 1) {
				fRec46_perm[j61] = fRec46_tmp[vsize + j61];
			}
			for (int j63 = 0; j63 < 4; j63 = j63 + 1) {
				fRec44_perm[j63] = fRec44_tmp[vsize + j63];
			}
			fYec14_idx_save = vsize;
			fYec15_idx_save = vsize;
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec43_perm[j53] = fRec43_tmp[vsize + j53];
			}
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec42_perm[j55] = fRec42_tmp[vsize + j55];
			}
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec40_perm[j57] = fRec40_tmp[vsize + j57];
			}
			fYec12_idx_save = vsize;
			fYec13_idx_save = vsize;
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec39_perm[j47] = fRec39_tmp[vsize + j47];
			}
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fRec38_perm[j49] = fRec38_tmp[vsize + j49];
			}
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec36_perm[j51] = fRec36_tmp[vsize + j51];
			}
			fYec10_idx_save = vsize;
			fYec11_idx_save = vsize;
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec35_perm[j41] = fRec35_tmp[vsize + j41];
			}
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec34_perm[j43] = fRec34_tmp[vsize + j43];
			}
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec32_perm[j45] = fRec32_tmp[vsize + j45];
			}
			fYec7_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec31_perm[j35] = fRec31_tmp[vsize + j35];
			}
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec30_perm[j37] = fRec30_tmp[vsize + j37];
			}
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec28_perm[j39] = fRec28_tmp[vsize + j39];
			}
			fYec5_idx_save = vsize;
			fYec6_idx_save = vsize;
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec27_perm[j29] = fRec27_tmp[vsize + j29];
			}
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec26_perm[j31] = fRec26_tmp[vsize + j31];
			}
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec24_perm[j33] = fRec24_tmp[vsize + j33];
			}
			fYec3_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec23_perm[j23] = fRec23_tmp[vsize + j23];
			}
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec22_perm[j25] = fRec22_tmp[vsize + j25];
			}
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec20_perm[j27] = fRec20_tmp[vsize + j27];
			}
			fYec1_idx_save = vsize;
			fYec2_idx_save = vsize;
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec15_perm[j9] = fRec15_tmp[vsize + j9];
			}
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec12_perm[j15] = fRec12_tmp[vsize + j15];
			}
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec10_perm[j21] = fRec10_tmp[vsize + j21];
			}
			for (int j65 = 0; j65 < 4; j65 = j65 + 1) {
				fRec2_perm[j65] = fRec2_tmp[vsize + j65];
			}
			for (int j67 = 0; j67 < 4; j67 = j67 + 1) {
				fRec3_perm[j67] = fRec3_tmp[vsize + j67];
			}
			for (int j69 = 0; j69 < 4; j69 = j69 + 1) {
				fRec4_perm[j69] = fRec4_tmp[vsize + j69];
			}
			for (int j71 = 0; j71 < 4; j71 = j71 + 1) {
				fRec5_perm[j71] = fRec5_tmp[vsize + j71];
			}
			for (int j73 = 0; j73 < 4; j73 = j73 + 1) {
				fRec6_perm[j73] = fRec6_tmp[vsize + j73];
			}
			for (int j75 = 0; j75 < 4; j75 = j75 + 1) {
				fRec7_perm[j75] = fRec7_tmp[vsize + j75];
			}
			for (int j77 = 0; j77 < 4; j77 = j77 + 1) {
				fRec8_perm[j77] = fRec8_tmp[vsize + j77];
			}
			for (int j79 = 0; j79 < 4; j79 = j79 + 1) {
				fRec9_perm[j79] = fRec9_tmp[vsize + j79];
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec61[i] = 1.0f - fZec60[i];
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec61[i] + 0.37f * (fRec3[i] + fRec4[i]) * fZec60[i]) + output0[i];
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec61[i] + 0.37f * (fRec3[i] - fRec4[i]) * fZec60[i]) + output1[i];
			}
		}
	}

};

struct RegisterReverb {
	static EffectBase* create() {
		// TODO maybe allocate this in a pool
		auto ptr = new Reverb();
		TKLB_PROFILER_MALLOC_L(ptr, sizeof(Reverb), vae::core::profiler::dspAllocator)
		return ptr;
	}
	RegisterReverb() {
		EffectsList::instance().add("Reverb", &create);
	}
} __registerReverb;

} } } // vae::core::effect

#endif
