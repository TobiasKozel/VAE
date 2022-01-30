/* ------------------------------------------------------------
name: "reverb"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a C:\dev\git\master\VAEG\VAE\src\core\dsp\vae_faust_arch.hpp -lang cpp -cm -mapp -es 1 -exp10 -vec -lv 0 -vs 32 -single -ftz 0 -mcd 16
------------------------------------------------------------ */

#ifndef  __VAEReverb_H__
#define  __VAEReverb_H__


#include "../vae_faust_common.hpp"
#include "../../../wrapped/vae_profiler.hpp"
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

static float VAEReverb_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS
#define FAUSTCLASS VAEReverb
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

class VAEReverb : public FaustBase {

 private:

	int fSampleRate;
	float fConst2;
	float fConst3;
	float fRec11_perm[4];
	float fConst4;
	float fConst5;
	FAUSTFLOAT fVslider0;
	float fConst9;
	FAUSTFLOAT fVslider1;
	float fRec10_perm[4];
	float fYec0[65536];
	int fYec0_idx;
	int fYec0_idx_save;
	int iConst16;
	float fYec1[16384];
	int fYec1_idx;
	int fYec1_idx_save;
	float fConst17;
	FAUSTFLOAT fVslider2;
	float fYec2[4096];
	int fYec2_idx;
	int fYec2_idx_save;
	int iConst20;
	float fRec8_perm[4];
	float fRec15_perm[4];
	float fConst24;
	float fRec14_perm[4];
	float fYec3[65536];
	int fYec3_idx;
	int fYec3_idx_save;
	int iConst31;
	float fYec4[8192];
	int fYec4_idx;
	int fYec4_idx_save;
	int iConst34;
	float fRec12_perm[4];
	float fRec19_perm[4];
	float fConst38;
	float fRec18_perm[4];
	float fYec5[65536];
	int fYec5_idx;
	int fYec5_idx_save;
	int iConst45;
	float fYec6[8192];
	int fYec6_idx;
	int fYec6_idx_save;
	int iConst48;
	float fRec16_perm[4];
	float fRec23_perm[4];
	float fConst52;
	float fRec22_perm[4];
	float fYec7[65536];
	int fYec7_idx;
	int fYec7_idx_save;
	int iConst59;
	float fYec8[8192];
	int fYec8_idx;
	int fYec8_idx_save;
	int iConst62;
	float fRec20_perm[4];
	float fRec27_perm[4];
	float fConst66;
	float fRec26_perm[4];
	float fYec9[32768];
	int fYec9_idx;
	int fYec9_idx_save;
	int iConst73;
	float fYec10[16384];
	int fYec10_idx;
	int fYec10_idx_save;
	float fYec11[4096];
	int fYec11_idx;
	int fYec11_idx_save;
	int iConst76;
	float fRec24_perm[4];
	float fRec31_perm[4];
	float fConst80;
	float fRec30_perm[4];
	float fYec12[32768];
	int fYec12_idx;
	int fYec12_idx_save;
	int iConst87;
	float fYec13[8192];
	int fYec13_idx;
	int fYec13_idx_save;
	int iConst90;
	float fRec28_perm[4];
	float fRec35_perm[4];
	float fConst94;
	float fRec34_perm[4];
	float fYec14[65536];
	int fYec14_idx;
	int fYec14_idx_save;
	int iConst101;
	float fYec15[8192];
	int fYec15_idx;
	int fYec15_idx_save;
	int iConst104;
	float fRec32_perm[4];
	float fRec39_perm[4];
	float fConst108;
	float fRec38_perm[4];
	float fYec16[32768];
	int fYec16_idx;
	int fYec16_idx_save;
	int iConst115;
	float fYec17[4096];
	int fYec17_idx;
	int fYec17_idx_save;
	int iConst118;
	float fRec36_perm[4];
	float fRec0_perm[4];
	float fRec1_perm[4];
	float fRec2_perm[4];
	float fRec3_perm[4];
	float fRec4_perm[4];
	float fRec5_perm[4];
	float fRec6_perm[4];
	float fRec7_perm[4];
	FAUSTFLOAT fVslider3;

 public:

	void metadata(Meta* m) {
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-a C:\dev\git\master\VAEG\VAE\src\core\dsp\vae_faust_arch.hpp -lang cpp -cm -mapp -es 1 -exp10 -vec -lv 0 -vs 32 -single -ftz 0 -mcd 16");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "reverb.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
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
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "reverb");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}

	static void classInit(int sample_rate) {
	}

	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = (1.0f / std::tan((1570.79639f / fConst0)));
		fConst2 = (1.0f / (fConst1 + 1.0f));
		fConst3 = (1.0f - fConst1);
		fConst4 = std::cos((18849.5566f / fConst0));
		fConst5 = (0.100000001f / fConst0);
		float fConst6 = ((0.219990999f * fConst0) + 0.5f);
		float fConst7 = float(int(fConst6));
		float fConst8 = ((fConst6 == fConst7) ? fConst6 : ((fConst6 >= 0.0f) ? fConst7 : (fConst7 + -1.0f)));
		fConst9 = (0.0f - (6.90775537f * fConst8));
		float fConst10 = ((0.0191229992f * fConst0) + 0.5f);
		float fConst11 = float(int(fConst10));
		float fConst12 = ((fConst10 == fConst11) ? fConst10 : ((fConst10 >= 0.0f) ? fConst11 : (fConst11 + -1.0f)));
		float fConst13 = (fConst8 - fConst12);
		float fConst14 = (1.44269502f * std::log(fConst13));
		float fConst15 = float(int(fConst14));
		iConst16 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst14 == fConst15) ? fConst14 : ((fConst14 >= 0.0f) ? (fConst15 + 1.0f) : fConst15)))))), std::max<float>(0.0f, fConst13)));
		fConst17 = (0.100000001f * fConst0);
		float fConst18 = (1.44269502f * std::log(fConst12));
		float fConst19 = float(int(fConst18));
		iConst20 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst18 == fConst19) ? fConst18 : ((fConst18 >= 0.0f) ? (fConst19 + 1.0f) : fConst19)))))), std::max<float>(0.0f, (fConst12 + -1.0f))));
		float fConst21 = ((0.256891012f * fConst0) + 0.5f);
		float fConst22 = float(int(fConst21));
		float fConst23 = ((fConst21 == fConst22) ? fConst21 : ((fConst21 >= 0.0f) ? fConst22 : (fConst22 + -1.0f)));
		fConst24 = (0.0f - (6.90775537f * fConst23));
		float fConst25 = ((0.0273330007f * fConst0) + 0.5f);
		float fConst26 = float(int(fConst25));
		float fConst27 = ((fConst25 == fConst26) ? fConst25 : ((fConst25 >= 0.0f) ? fConst26 : (fConst26 + -1.0f)));
		float fConst28 = (fConst23 - fConst27);
		float fConst29 = (1.44269502f * std::log(fConst28));
		float fConst30 = float(int(fConst29));
		iConst31 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst29 == fConst30) ? fConst29 : ((fConst29 >= 0.0f) ? (fConst30 + 1.0f) : fConst30)))))), std::max<float>(0.0f, fConst28)));
		float fConst32 = (1.44269502f * std::log(fConst27));
		float fConst33 = float(int(fConst32));
		iConst34 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst32 == fConst33) ? fConst32 : ((fConst32 >= 0.0f) ? (fConst33 + 1.0f) : fConst33)))))), std::max<float>(0.0f, (fConst27 + -1.0f))));
		float fConst35 = ((0.192303002f * fConst0) + 0.5f);
		float fConst36 = float(int(fConst35));
		float fConst37 = ((fConst35 == fConst36) ? fConst35 : ((fConst35 >= 0.0f) ? fConst36 : (fConst36 + -1.0f)));
		fConst38 = (0.0f - (6.90775537f * fConst37));
		float fConst39 = ((0.0292910002f * fConst0) + 0.5f);
		float fConst40 = float(int(fConst39));
		float fConst41 = ((fConst39 == fConst40) ? fConst39 : ((fConst39 >= 0.0f) ? fConst40 : (fConst40 + -1.0f)));
		float fConst42 = (fConst37 - fConst41);
		float fConst43 = (1.44269502f * std::log(fConst42));
		float fConst44 = float(int(fConst43));
		iConst45 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst43 == fConst44) ? fConst43 : ((fConst43 >= 0.0f) ? (fConst44 + 1.0f) : fConst44)))))), std::max<float>(0.0f, fConst42)));
		float fConst46 = (1.44269502f * std::log(fConst41));
		float fConst47 = float(int(fConst46));
		iConst48 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst46 == fConst47) ? fConst46 : ((fConst46 >= 0.0f) ? (fConst47 + 1.0f) : fConst47)))))), std::max<float>(0.0f, (fConst41 + -1.0f))));
		float fConst49 = ((0.210389003f * fConst0) + 0.5f);
		float fConst50 = float(int(fConst49));
		float fConst51 = ((fConst49 == fConst50) ? fConst49 : ((fConst49 >= 0.0f) ? fConst50 : (fConst50 + -1.0f)));
		fConst52 = (0.0f - (6.90775537f * fConst51));
		float fConst53 = ((0.0244210009f * fConst0) + 0.5f);
		float fConst54 = float(int(fConst53));
		float fConst55 = ((fConst53 == fConst54) ? fConst53 : ((fConst53 >= 0.0f) ? fConst54 : (fConst54 + -1.0f)));
		float fConst56 = (fConst51 - fConst55);
		float fConst57 = (1.44269502f * std::log(fConst56));
		float fConst58 = float(int(fConst57));
		iConst59 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst57 == fConst58) ? fConst57 : ((fConst57 >= 0.0f) ? (fConst58 + 1.0f) : fConst58)))))), std::max<float>(0.0f, fConst56)));
		float fConst60 = (1.44269502f * std::log(fConst55));
		float fConst61 = float(int(fConst60));
		iConst62 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst60 == fConst61) ? fConst60 : ((fConst60 >= 0.0f) ? (fConst61 + 1.0f) : fConst61)))))), std::max<float>(0.0f, (fConst55 + -1.0f))));
		float fConst63 = ((0.125f * fConst0) + 0.5f);
		float fConst64 = float(int(fConst63));
		float fConst65 = ((fConst63 == fConst64) ? fConst63 : ((fConst63 >= 0.0f) ? fConst64 : (fConst64 + -1.0f)));
		fConst66 = (0.0f - (6.90775537f * fConst65));
		float fConst67 = ((0.0134579996f * fConst0) + 0.5f);
		float fConst68 = float(int(fConst67));
		float fConst69 = ((fConst67 == fConst68) ? fConst67 : ((fConst67 >= 0.0f) ? fConst68 : (fConst68 + -1.0f)));
		float fConst70 = (fConst65 - fConst69);
		float fConst71 = (1.44269502f * std::log(fConst70));
		float fConst72 = float(int(fConst71));
		iConst73 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst71 == fConst72) ? fConst71 : ((fConst71 >= 0.0f) ? (fConst72 + 1.0f) : fConst72)))))), std::max<float>(0.0f, fConst70)));
		float fConst74 = (1.44269502f * std::log(fConst69));
		float fConst75 = float(int(fConst74));
		iConst76 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst74 == fConst75) ? fConst74 : ((fConst74 >= 0.0f) ? (fConst75 + 1.0f) : fConst75)))))), std::max<float>(0.0f, (fConst69 + -1.0f))));
		float fConst77 = ((0.127837002f * fConst0) + 0.5f);
		float fConst78 = float(int(fConst77));
		float fConst79 = ((fConst77 == fConst78) ? fConst77 : ((fConst77 >= 0.0f) ? fConst78 : (fConst78 + -1.0f)));
		fConst80 = (0.0f - (6.90775537f * fConst79));
		float fConst81 = ((0.0316039994f * fConst0) + 0.5f);
		float fConst82 = float(int(fConst81));
		float fConst83 = ((fConst81 == fConst82) ? fConst81 : ((fConst81 >= 0.0f) ? fConst82 : (fConst82 + -1.0f)));
		float fConst84 = (fConst79 - fConst83);
		float fConst85 = (1.44269502f * std::log(fConst84));
		float fConst86 = float(int(fConst85));
		iConst87 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst85 == fConst86) ? fConst85 : ((fConst85 >= 0.0f) ? (fConst86 + 1.0f) : fConst86)))))), std::max<float>(0.0f, fConst84)));
		float fConst88 = (1.44269502f * std::log(fConst83));
		float fConst89 = float(int(fConst88));
		iConst90 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst88 == fConst89) ? fConst88 : ((fConst88 >= 0.0f) ? (fConst89 + 1.0f) : fConst89)))))), std::max<float>(0.0f, (fConst83 + -1.0f))));
		float fConst91 = ((0.174713001f * fConst0) + 0.5f);
		float fConst92 = float(int(fConst91));
		float fConst93 = ((fConst91 == fConst92) ? fConst91 : ((fConst91 >= 0.0f) ? fConst92 : (fConst92 + -1.0f)));
		fConst94 = (0.0f - (6.90775537f * fConst93));
		float fConst95 = ((0.0229039993f * fConst0) + 0.5f);
		float fConst96 = float(int(fConst95));
		float fConst97 = ((fConst95 == fConst96) ? fConst95 : ((fConst95 >= 0.0f) ? fConst96 : (fConst96 + -1.0f)));
		float fConst98 = (fConst93 - fConst97);
		float fConst99 = (1.44269502f * std::log(fConst98));
		float fConst100 = float(int(fConst99));
		iConst101 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst99 == fConst100) ? fConst99 : ((fConst99 >= 0.0f) ? (fConst100 + 1.0f) : fConst100)))))), std::max<float>(0.0f, fConst98)));
		float fConst102 = (1.44269502f * std::log(fConst97));
		float fConst103 = float(int(fConst102));
		iConst104 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst102 == fConst103) ? fConst102 : ((fConst102 >= 0.0f) ? (fConst103 + 1.0f) : fConst103)))))), std::max<float>(0.0f, (fConst97 + -1.0f))));
		float fConst105 = ((0.153128996f * fConst0) + 0.5f);
		float fConst106 = float(int(fConst105));
		float fConst107 = ((fConst105 == fConst106) ? fConst105 : ((fConst105 >= 0.0f) ? fConst106 : (fConst106 + -1.0f)));
		fConst108 = (0.0f - (6.90775537f * fConst107));
		float fConst109 = ((0.0203460008f * fConst0) + 0.5f);
		float fConst110 = float(int(fConst109));
		float fConst111 = ((fConst109 == fConst110) ? fConst109 : ((fConst109 >= 0.0f) ? fConst110 : (fConst110 + -1.0f)));
		float fConst112 = (fConst107 - fConst111);
		float fConst113 = (1.44269502f * std::log(fConst112));
		float fConst114 = float(int(fConst113));
		iConst115 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst113 == fConst114) ? fConst113 : ((fConst113 >= 0.0f) ? (fConst114 + 1.0f) : fConst114)))))), std::max<float>(0.0f, fConst112)));
		float fConst116 = (1.44269502f * std::log(fConst111));
		float fConst117 = float(int(fConst116));
		iConst118 = int(std::min<float>(float(int(std::pow(2.0f, std::max<float>(1.0f, ((fConst116 == fConst117) ? fConst116 : ((fConst116 >= 0.0f) ? (fConst117 + 1.0f) : fConst117)))))), std::max<float>(0.0f, (fConst111 + -1.0f))));
	}

	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.5f);
		fVslider1 = FAUSTFLOAT(0.5f);
		fVslider2 = FAUSTFLOAT(0.5f);
		fVslider3 = FAUSTFLOAT(1.0f);
	}

	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 4); l0 = (l0 + 1)) {
			fRec11_perm[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 4); l1 = (l1 + 1)) {
			fRec10_perm[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 65536); l2 = (l2 + 1)) {
			fYec0[l2] = 0.0f;
		}
		fYec0_idx = 0;
		fYec0_idx_save = 0;
		for (int l3 = 0; (l3 < 16384); l3 = (l3 + 1)) {
			fYec1[l3] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
		for (int l4 = 0; (l4 < 4096); l4 = (l4 + 1)) {
			fYec2[l4] = 0.0f;
		}
		fYec2_idx = 0;
		fYec2_idx_save = 0;
		for (int l5 = 0; (l5 < 4); l5 = (l5 + 1)) {
			fRec8_perm[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 4); l6 = (l6 + 1)) {
			fRec15_perm[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 4); l7 = (l7 + 1)) {
			fRec14_perm[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 65536); l8 = (l8 + 1)) {
			fYec3[l8] = 0.0f;
		}
		fYec3_idx = 0;
		fYec3_idx_save = 0;
		for (int l9 = 0; (l9 < 8192); l9 = (l9 + 1)) {
			fYec4[l9] = 0.0f;
		}
		fYec4_idx = 0;
		fYec4_idx_save = 0;
		for (int l10 = 0; (l10 < 4); l10 = (l10 + 1)) {
			fRec12_perm[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 4); l11 = (l11 + 1)) {
			fRec19_perm[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 4); l12 = (l12 + 1)) {
			fRec18_perm[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 65536); l13 = (l13 + 1)) {
			fYec5[l13] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		for (int l14 = 0; (l14 < 8192); l14 = (l14 + 1)) {
			fYec6[l14] = 0.0f;
		}
		fYec6_idx = 0;
		fYec6_idx_save = 0;
		for (int l15 = 0; (l15 < 4); l15 = (l15 + 1)) {
			fRec16_perm[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 4); l16 = (l16 + 1)) {
			fRec23_perm[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 4); l17 = (l17 + 1)) {
			fRec22_perm[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 65536); l18 = (l18 + 1)) {
			fYec7[l18] = 0.0f;
		}
		fYec7_idx = 0;
		fYec7_idx_save = 0;
		for (int l19 = 0; (l19 < 8192); l19 = (l19 + 1)) {
			fYec8[l19] = 0.0f;
		}
		fYec8_idx = 0;
		fYec8_idx_save = 0;
		for (int l20 = 0; (l20 < 4); l20 = (l20 + 1)) {
			fRec20_perm[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 4); l21 = (l21 + 1)) {
			fRec27_perm[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 4); l22 = (l22 + 1)) {
			fRec26_perm[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 32768); l23 = (l23 + 1)) {
			fYec9[l23] = 0.0f;
		}
		fYec9_idx = 0;
		fYec9_idx_save = 0;
		for (int l24 = 0; (l24 < 16384); l24 = (l24 + 1)) {
			fYec10[l24] = 0.0f;
		}
		fYec10_idx = 0;
		fYec10_idx_save = 0;
		for (int l25 = 0; (l25 < 4096); l25 = (l25 + 1)) {
			fYec11[l25] = 0.0f;
		}
		fYec11_idx = 0;
		fYec11_idx_save = 0;
		for (int l26 = 0; (l26 < 4); l26 = (l26 + 1)) {
			fRec24_perm[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 4); l27 = (l27 + 1)) {
			fRec31_perm[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 4); l28 = (l28 + 1)) {
			fRec30_perm[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 32768); l29 = (l29 + 1)) {
			fYec12[l29] = 0.0f;
		}
		fYec12_idx = 0;
		fYec12_idx_save = 0;
		for (int l30 = 0; (l30 < 8192); l30 = (l30 + 1)) {
			fYec13[l30] = 0.0f;
		}
		fYec13_idx = 0;
		fYec13_idx_save = 0;
		for (int l31 = 0; (l31 < 4); l31 = (l31 + 1)) {
			fRec28_perm[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 4); l32 = (l32 + 1)) {
			fRec35_perm[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 4); l33 = (l33 + 1)) {
			fRec34_perm[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 65536); l34 = (l34 + 1)) {
			fYec14[l34] = 0.0f;
		}
		fYec14_idx = 0;
		fYec14_idx_save = 0;
		for (int l35 = 0; (l35 < 8192); l35 = (l35 + 1)) {
			fYec15[l35] = 0.0f;
		}
		fYec15_idx = 0;
		fYec15_idx_save = 0;
		for (int l36 = 0; (l36 < 4); l36 = (l36 + 1)) {
			fRec32_perm[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 4); l37 = (l37 + 1)) {
			fRec39_perm[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 4); l38 = (l38 + 1)) {
			fRec38_perm[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 32768); l39 = (l39 + 1)) {
			fYec16[l39] = 0.0f;
		}
		fYec16_idx = 0;
		fYec16_idx_save = 0;
		for (int l40 = 0; (l40 < 4096); l40 = (l40 + 1)) {
			fYec17[l40] = 0.0f;
		}
		fYec17_idx = 0;
		fYec17_idx_save = 0;
		for (int l41 = 0; (l41 < 4); l41 = (l41 + 1)) {
			fRec36_perm[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 4); l42 = (l42 + 1)) {
			fRec0_perm[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 4); l43 = (l43 + 1)) {
			fRec1_perm[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 4); l44 = (l44 + 1)) {
			fRec2_perm[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 4); l45 = (l45 + 1)) {
			fRec3_perm[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 4); l46 = (l46 + 1)) {
			fRec4_perm[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 4); l47 = (l47 + 1)) {
			fRec5_perm[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 4); l48 = (l48 + 1)) {
			fRec6_perm[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 4); l49 = (l49 + 1)) {
			fRec7_perm[l49] = 0.0f;
		}
	}

	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}

	virtual VAEReverb* clone() {
		return new VAEReverb();
	}

	virtual int getSampleRate() {
		return fSampleRate;
	}

	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("reverb");
		ui_interface->addVerticalSlider("fb1", &fVslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addVerticalSlider("fb2", &fVslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addVerticalSlider("mix", &fVslider3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addVerticalSlider("pre", &fVslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}

	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fRec11_tmp[36];
		float* fRec11 = &fRec11_tmp[4];
		float fSlow0 = (fConst5 / float(fVslider0));
		float fSlow1 = std::exp((fSlow0 * fConst9));
		float fSlow2 = VAEReverb_faustpower2_f(fSlow1);
		float fSlow3 = (1.0f - (fConst4 * fSlow2));
		float fSlow4 = (1.0f - fSlow2);
		float fSlow5 = (fSlow3 / fSlow4);
		float fSlow6 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow3) / VAEReverb_faustpower2_f(fSlow4)) + -1.0f)));
		float fSlow7 = (fSlow5 - fSlow6);
		float fSlow8 = (fConst5 / float(fVslider1));
		float fSlow9 = ((std::exp((fSlow8 * fConst9)) / fSlow1) + -1.0f);
		float fSlow10 = (fSlow6 + (1.0f - fSlow5));
		float fRec10_tmp[36];
		float* fRec10 = &fRec10_tmp[4];
		int iSlow11 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst17 * float(fVslider2)))));
		float fZec0[32];
		float fRec8_tmp[36];
		float* fRec8 = &fRec8_tmp[4];
		float fRec9[32];
		float fRec15_tmp[36];
		float* fRec15 = &fRec15_tmp[4];
		float fSlow12 = std::exp((fSlow0 * fConst24));
		float fSlow13 = VAEReverb_faustpower2_f(fSlow12);
		float fSlow14 = (1.0f - (fConst4 * fSlow13));
		float fSlow15 = (1.0f - fSlow13);
		float fSlow16 = (fSlow14 / fSlow15);
		float fSlow17 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow14) / VAEReverb_faustpower2_f(fSlow15)) + -1.0f)));
		float fSlow18 = (fSlow16 - fSlow17);
		float fSlow19 = ((std::exp((fSlow8 * fConst24)) / fSlow12) + -1.0f);
		float fSlow20 = (fSlow17 + (1.0f - fSlow16));
		float fRec14_tmp[36];
		float* fRec14 = &fRec14_tmp[4];
		float fRec12_tmp[36];
		float* fRec12 = &fRec12_tmp[4];
		float fRec13[32];
		float fRec19_tmp[36];
		float* fRec19 = &fRec19_tmp[4];
		float fSlow21 = std::exp((fSlow0 * fConst38));
		float fSlow22 = VAEReverb_faustpower2_f(fSlow21);
		float fSlow23 = (1.0f - (fConst4 * fSlow22));
		float fSlow24 = (1.0f - fSlow22);
		float fSlow25 = (fSlow23 / fSlow24);
		float fSlow26 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow23) / VAEReverb_faustpower2_f(fSlow24)) + -1.0f)));
		float fSlow27 = (fSlow25 - fSlow26);
		float fSlow28 = ((std::exp((fSlow8 * fConst38)) / fSlow21) + -1.0f);
		float fSlow29 = (fSlow26 + (1.0f - fSlow25));
		float fRec18_tmp[36];
		float* fRec18 = &fRec18_tmp[4];
		float fRec16_tmp[36];
		float* fRec16 = &fRec16_tmp[4];
		float fRec17[32];
		float fRec23_tmp[36];
		float* fRec23 = &fRec23_tmp[4];
		float fSlow30 = std::exp((fSlow0 * fConst52));
		float fSlow31 = VAEReverb_faustpower2_f(fSlow30);
		float fSlow32 = (1.0f - (fConst4 * fSlow31));
		float fSlow33 = (1.0f - fSlow31);
		float fSlow34 = (fSlow32 / fSlow33);
		float fSlow35 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow32) / VAEReverb_faustpower2_f(fSlow33)) + -1.0f)));
		float fSlow36 = (fSlow34 - fSlow35);
		float fSlow37 = ((std::exp((fSlow8 * fConst52)) / fSlow30) + -1.0f);
		float fSlow38 = (fSlow35 + (1.0f - fSlow34));
		float fRec22_tmp[36];
		float* fRec22 = &fRec22_tmp[4];
		float fRec20_tmp[36];
		float* fRec20 = &fRec20_tmp[4];
		float fRec21[32];
		float fRec27_tmp[36];
		float* fRec27 = &fRec27_tmp[4];
		float fSlow39 = std::exp((fSlow0 * fConst66));
		float fSlow40 = VAEReverb_faustpower2_f(fSlow39);
		float fSlow41 = (1.0f - (fConst4 * fSlow40));
		float fSlow42 = (1.0f - fSlow40);
		float fSlow43 = (fSlow41 / fSlow42);
		float fSlow44 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow41) / VAEReverb_faustpower2_f(fSlow42)) + -1.0f)));
		float fSlow45 = (fSlow43 - fSlow44);
		float fSlow46 = ((std::exp((fSlow8 * fConst66)) / fSlow39) + -1.0f);
		float fSlow47 = (fSlow44 + (1.0f - fSlow43));
		float fRec26_tmp[36];
		float* fRec26 = &fRec26_tmp[4];
		float fZec1[32];
		float fRec24_tmp[36];
		float* fRec24 = &fRec24_tmp[4];
		float fRec25[32];
		float fRec31_tmp[36];
		float* fRec31 = &fRec31_tmp[4];
		float fSlow48 = std::exp((fSlow0 * fConst80));
		float fSlow49 = VAEReverb_faustpower2_f(fSlow48);
		float fSlow50 = (1.0f - (fConst4 * fSlow49));
		float fSlow51 = (1.0f - fSlow49);
		float fSlow52 = (fSlow50 / fSlow51);
		float fSlow53 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow50) / VAEReverb_faustpower2_f(fSlow51)) + -1.0f)));
		float fSlow54 = (fSlow52 - fSlow53);
		float fSlow55 = ((std::exp((fSlow8 * fConst80)) / fSlow48) + -1.0f);
		float fSlow56 = (fSlow53 + (1.0f - fSlow52));
		float fRec30_tmp[36];
		float* fRec30 = &fRec30_tmp[4];
		float fRec28_tmp[36];
		float* fRec28 = &fRec28_tmp[4];
		float fRec29[32];
		float fRec35_tmp[36];
		float* fRec35 = &fRec35_tmp[4];
		float fSlow57 = std::exp((fSlow0 * fConst94));
		float fSlow58 = VAEReverb_faustpower2_f(fSlow57);
		float fSlow59 = (1.0f - (fConst4 * fSlow58));
		float fSlow60 = (1.0f - fSlow58);
		float fSlow61 = (fSlow59 / fSlow60);
		float fSlow62 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow59) / VAEReverb_faustpower2_f(fSlow60)) + -1.0f)));
		float fSlow63 = (fSlow61 - fSlow62);
		float fSlow64 = ((std::exp((fSlow8 * fConst94)) / fSlow57) + -1.0f);
		float fSlow65 = (fSlow62 + (1.0f - fSlow61));
		float fRec34_tmp[36];
		float* fRec34 = &fRec34_tmp[4];
		float fRec32_tmp[36];
		float* fRec32 = &fRec32_tmp[4];
		float fRec33[32];
		float fRec39_tmp[36];
		float* fRec39 = &fRec39_tmp[4];
		float fSlow66 = std::exp((fSlow0 * fConst108));
		float fSlow67 = VAEReverb_faustpower2_f(fSlow66);
		float fSlow68 = (1.0f - (fConst4 * fSlow67));
		float fSlow69 = (1.0f - fSlow67);
		float fSlow70 = (fSlow68 / fSlow69);
		float fSlow71 = std::sqrt(std::max<float>(0.0f, ((VAEReverb_faustpower2_f(fSlow68) / VAEReverb_faustpower2_f(fSlow69)) + -1.0f)));
		float fSlow72 = (fSlow70 - fSlow71);
		float fSlow73 = ((std::exp((fSlow8 * fConst108)) / fSlow66) + -1.0f);
		float fSlow74 = (fSlow71 + (1.0f - fSlow70));
		float fRec38_tmp[36];
		float* fRec38 = &fRec38_tmp[4];
		float fRec36_tmp[36];
		float* fRec36 = &fRec36_tmp[4];
		float fRec37[32];
		float fZec2[32];
		float fZec3[32];
		float fRec0_tmp[36];
		float* fRec0 = &fRec0_tmp[4];
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fZec4[32];
		float fZec5[32];
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		float fZec6[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fRec5_tmp[36];
		float* fRec5 = &fRec5_tmp[4];
		float fZec7[32];
		float fZec8[32];
		float fRec6_tmp[36];
		float* fRec6 = &fRec6_tmp[4];
		float fRec7_tmp[36];
		float* fRec7 = &fRec7_tmp[4];
		float fSlow75 = float(fVslider3);
		float fSlow76 = (0.370000005f * fSlow75);
		float fSlow77 = (1.0f - fSlow75);
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; (vindex <= (count - 32)); vindex = (vindex + 32)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 32;
			/* Vectorizable loop 0 */
			/* Pre code */
			fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 16383);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec10[((i + fYec10_idx) & 16383)] = float(input0[i]);
			}
			/* Post code */
			fYec10_idx_save = vsize;
			/* Vectorizable loop 1 */
			/* Pre code */
			fYec1_idx = ((fYec1_idx + fYec1_idx_save) & 16383);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec1[((i + fYec1_idx) & 16383)] = float(input1[i]);
			}
			/* Post code */
			fYec1_idx_save = vsize;
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec1[i] = (0.300000012f * fYec10[(((i + fYec10_idx) - iSlow11) & 16383)]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec0[i] = (0.300000012f * fYec1[(((i + fYec1_idx) - iSlow11) & 16383)]);
			}
			/* Recursive loop 4 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec11_tmp[j0] = fRec11_perm[j0];
			}
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec10_tmp[j2] = fRec10_perm[j2];
			}
			fYec0_idx = ((fYec0_idx + fYec0_idx_save) & 65535);
			fYec2_idx = ((fYec2_idx + fYec2_idx_save) & 4095);
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec8_tmp[j4] = fRec8_perm[j4];
			}
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				fRec15_tmp[j6] = fRec15_perm[j6];
			}
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec14_tmp[j8] = fRec14_perm[j8];
			}
			fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 65535);
			fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 8191);
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec12_tmp[j10] = fRec12_perm[j10];
			}
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				fRec19_tmp[j12] = fRec19_perm[j12];
			}
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec18_tmp[j14] = fRec18_perm[j14];
			}
			fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 65535);
			fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 8191);
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fRec16_tmp[j16] = fRec16_perm[j16];
			}
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec23_tmp[j18] = fRec23_perm[j18];
			}
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec22_tmp[j20] = fRec22_perm[j20];
			}
			fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 65535);
			fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 8191);
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				fRec20_tmp[j22] = fRec20_perm[j22];
			}
			for (int j24 = 0; (j24 < 4); j24 = (j24 + 1)) {
				fRec27_tmp[j24] = fRec27_perm[j24];
			}
			for (int j26 = 0; (j26 < 4); j26 = (j26 + 1)) {
				fRec26_tmp[j26] = fRec26_perm[j26];
			}
			fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 32767);
			fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 4095);
			for (int j28 = 0; (j28 < 4); j28 = (j28 + 1)) {
				fRec24_tmp[j28] = fRec24_perm[j28];
			}
			for (int j30 = 0; (j30 < 4); j30 = (j30 + 1)) {
				fRec31_tmp[j30] = fRec31_perm[j30];
			}
			for (int j32 = 0; (j32 < 4); j32 = (j32 + 1)) {
				fRec30_tmp[j32] = fRec30_perm[j32];
			}
			fYec12_idx = ((fYec12_idx + fYec12_idx_save) & 32767);
			fYec13_idx = ((fYec13_idx + fYec13_idx_save) & 8191);
			for (int j34 = 0; (j34 < 4); j34 = (j34 + 1)) {
				fRec28_tmp[j34] = fRec28_perm[j34];
			}
			for (int j36 = 0; (j36 < 4); j36 = (j36 + 1)) {
				fRec35_tmp[j36] = fRec35_perm[j36];
			}
			for (int j38 = 0; (j38 < 4); j38 = (j38 + 1)) {
				fRec34_tmp[j38] = fRec34_perm[j38];
			}
			fYec14_idx = ((fYec14_idx + fYec14_idx_save) & 65535);
			fYec15_idx = ((fYec15_idx + fYec15_idx_save) & 8191);
			for (int j40 = 0; (j40 < 4); j40 = (j40 + 1)) {
				fRec32_tmp[j40] = fRec32_perm[j40];
			}
			for (int j42 = 0; (j42 < 4); j42 = (j42 + 1)) {
				fRec39_tmp[j42] = fRec39_perm[j42];
			}
			for (int j44 = 0; (j44 < 4); j44 = (j44 + 1)) {
				fRec38_tmp[j44] = fRec38_perm[j44];
			}
			fYec16_idx = ((fYec16_idx + fYec16_idx_save) & 32767);
			fYec17_idx = ((fYec17_idx + fYec17_idx_save) & 4095);
			for (int j46 = 0; (j46 < 4); j46 = (j46 + 1)) {
				fRec36_tmp[j46] = fRec36_perm[j46];
			}
			for (int j48 = 0; (j48 < 4); j48 = (j48 + 1)) {
				fRec0_tmp[j48] = fRec0_perm[j48];
			}
			for (int j50 = 0; (j50 < 4); j50 = (j50 + 1)) {
				fRec1_tmp[j50] = fRec1_perm[j50];
			}
			for (int j52 = 0; (j52 < 4); j52 = (j52 + 1)) {
				fRec2_tmp[j52] = fRec2_perm[j52];
			}
			for (int j54 = 0; (j54 < 4); j54 = (j54 + 1)) {
				fRec3_tmp[j54] = fRec3_perm[j54];
			}
			for (int j56 = 0; (j56 < 4); j56 = (j56 + 1)) {
				fRec4_tmp[j56] = fRec4_perm[j56];
			}
			for (int j58 = 0; (j58 < 4); j58 = (j58 + 1)) {
				fRec5_tmp[j58] = fRec5_perm[j58];
			}
			for (int j60 = 0; (j60 < 4); j60 = (j60 + 1)) {
				fRec6_tmp[j60] = fRec6_perm[j60];
			}
			for (int j62 = 0; (j62 < 4); j62 = (j62 + 1)) {
				fRec7_tmp[j62] = fRec7_perm[j62];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec11[i] = (fConst2 * ((fRec7[(i - 1)] + fRec7[(i - 2)]) - (fConst3 * fRec11[(i - 1)])));
				fRec10[i] = ((fRec10[(i - 1)] * fSlow7) + ((fSlow1 * (fRec7[(i - 1)] + (fRec11[i] * fSlow9))) * fSlow10));
				fYec0[((i + fYec0_idx) & 65535)] = ((0.353553385f * fRec10[i]) + 9.99999968e-21f);
				fYec2[((i + fYec2_idx) & 4095)] = (((0.600000024f * fRec8[(i - 1)]) + fYec0[(((i + fYec0_idx) - iConst16) & 65535)]) - fZec0[i]);
				fRec8[i] = fYec2[(((i + fYec2_idx) - iConst20) & 4095)];
				fRec9[i] = (0.0f - (0.600000024f * fYec2[((i + fYec2_idx) & 4095)]));
				fRec15[i] = (fConst2 * ((fRec3[(i - 1)] + fRec3[(i - 2)]) - (fConst3 * fRec15[(i - 1)])));
				fRec14[i] = ((fRec14[(i - 1)] * fSlow18) + ((fSlow12 * (fRec3[(i - 1)] + (fRec15[i] * fSlow19))) * fSlow20));
				fYec3[((i + fYec3_idx) & 65535)] = ((0.353553385f * fRec14[i]) + 9.99999968e-21f);
				fYec4[((i + fYec4_idx) & 8191)] = (((0.600000024f * fRec12[(i - 1)]) + fYec3[(((i + fYec3_idx) - iConst31) & 65535)]) - fZec0[i]);
				fRec12[i] = fYec4[(((i + fYec4_idx) - iConst34) & 8191)];
				fRec13[i] = (0.0f - (0.600000024f * fYec4[((i + fYec4_idx) & 8191)]));
				fRec19[i] = (0.0f - (fConst2 * ((fConst3 * fRec19[(i - 1)]) - (fRec5[(i - 1)] + fRec5[(i - 2)]))));
				fRec18[i] = ((fRec18[(i - 1)] * fSlow27) + ((fSlow21 * (fRec5[(i - 1)] + (fRec19[i] * fSlow28))) * fSlow29));
				fYec5[((i + fYec5_idx) & 65535)] = ((0.353553385f * fRec18[i]) + 9.99999968e-21f);
				fYec6[((i + fYec6_idx) & 8191)] = (fYec5[(((i + fYec5_idx) - iConst45) & 65535)] + (fZec0[i] + (0.600000024f * fRec16[(i - 1)])));
				fRec16[i] = fYec6[(((i + fYec6_idx) - iConst48) & 8191)];
				fRec17[i] = (0.0f - (0.600000024f * fYec6[((i + fYec6_idx) & 8191)]));
				fRec23[i] = (fConst2 * ((fRec1[(i - 1)] + fRec1[(i - 2)]) - (fConst3 * fRec23[(i - 1)])));
				fRec22[i] = ((fRec22[(i - 1)] * fSlow36) + ((fSlow30 * (fRec1[(i - 1)] + (fRec23[i] * fSlow37))) * fSlow38));
				fYec7[((i + fYec7_idx) & 65535)] = ((0.353553385f * fRec22[i]) + 9.99999968e-21f);
				fYec8[((i + fYec8_idx) & 8191)] = (fYec7[(((i + fYec7_idx) - iConst59) & 65535)] + ((0.600000024f * fRec20[(i - 1)]) + fZec0[i]));
				fRec20[i] = fYec8[(((i + fYec8_idx) - iConst62) & 8191)];
				fRec21[i] = (0.0f - (0.600000024f * fYec8[((i + fYec8_idx) & 8191)]));
				fRec27[i] = (fConst2 * ((fRec6[(i - 1)] + fRec6[(i - 2)]) - (fConst3 * fRec27[(i - 1)])));
				fRec26[i] = ((fRec26[(i - 1)] * fSlow45) + ((fSlow39 * (fRec6[(i - 1)] + (fRec27[i] * fSlow46))) * fSlow47));
				fYec9[((i + fYec9_idx) & 32767)] = ((0.353553385f * fRec26[i]) + 9.99999968e-21f);
				fYec11[((i + fYec11_idx) & 4095)] = (fYec9[(((i + fYec9_idx) - iConst73) & 32767)] - (fZec1[i] + (0.600000024f * fRec24[(i - 1)])));
				fRec24[i] = fYec11[(((i + fYec11_idx) - iConst76) & 4095)];
				fRec25[i] = (0.600000024f * fYec11[((i + fYec11_idx) & 4095)]);
				fRec31[i] = (0.0f - (fConst2 * ((fConst3 * fRec31[(i - 1)]) - (fRec2[(i - 1)] + fRec2[(i - 2)]))));
				fRec30[i] = ((fRec30[(i - 1)] * fSlow54) + ((fSlow48 * (fRec2[(i - 1)] + (fRec31[i] * fSlow55))) * fSlow56));
				fYec12[((i + fYec12_idx) & 32767)] = ((0.353553385f * fRec30[i]) + 9.99999968e-21f);
				fYec13[((i + fYec13_idx) & 8191)] = (fYec12[(((i + fYec12_idx) - iConst87) & 32767)] - (fZec1[i] + (0.600000024f * fRec28[(i - 1)])));
				fRec28[i] = fYec13[(((i + fYec13_idx) - iConst90) & 8191)];
				fRec29[i] = (0.600000024f * fYec13[((i + fYec13_idx) & 8191)]);
				fRec35[i] = (fConst2 * ((fRec4[(i - 1)] + fRec4[(i - 2)]) - (fConst3 * fRec35[(i - 1)])));
				fRec34[i] = ((fRec34[(i - 1)] * fSlow63) + ((fSlow57 * (fRec4[(i - 1)] + (fRec35[i] * fSlow64))) * fSlow65));
				fYec14[((i + fYec14_idx) & 65535)] = ((0.353553385f * fRec34[i]) + 9.99999968e-21f);
				fYec15[((i + fYec15_idx) & 8191)] = ((fZec1[i] + fYec14[(((i + fYec14_idx) - iConst101) & 65535)]) - (0.600000024f * fRec32[(i - 1)]));
				fRec32[i] = fYec15[(((i + fYec15_idx) - iConst104) & 8191)];
				fRec33[i] = (0.600000024f * fYec15[((i + fYec15_idx) & 8191)]);
				fRec39[i] = (fConst2 * ((fRec0[(i - 1)] + fRec0[(i - 2)]) - (fConst3 * fRec39[(i - 1)])));
				fRec38[i] = ((fRec38[(i - 1)] * fSlow72) + ((fSlow66 * (fRec0[(i - 1)] + (fRec39[i] * fSlow73))) * fSlow74));
				fYec16[((i + fYec16_idx) & 32767)] = ((0.353553385f * fRec38[i]) + 9.99999968e-21f);
				fYec17[((i + fYec17_idx) & 4095)] = ((fYec16[(((i + fYec16_idx) - iConst115) & 32767)] + fZec1[i]) - (0.600000024f * fRec36[(i - 1)]));
				fRec36[i] = fYec17[(((i + fYec17_idx) - iConst118) & 4095)];
				fRec37[i] = (0.600000024f * fYec17[((i + fYec17_idx) & 4095)]);
				fZec2[i] = (fRec37[i] + fRec29[i]);
				fZec3[i] = (fRec25[i] + (fRec33[i] + fZec2[i]));
				fRec0[i] = (fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec9[i] + (fRec17[i] + (fRec21[i] + fZec3[i]))))))))))));
				fRec1[i] = ((fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + fZec3[i])))) - (fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec13[i] + (fRec9[i] + (fRec21[i] + fRec17[i]))))))));
				fZec4[i] = (fRec37[i] + fRec33[i]);
				fZec5[i] = (fRec29[i] + fRec25[i]);
				fRec2[i] = ((fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + (fRec17[i] + (fRec21[i] + fZec4[i])))))) - (fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec13[i] + (fRec9[i] + fZec5[i])))))));
				fRec3[i] = ((fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec9[i] + fZec4[i])))))) - (fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec17[i] + (fRec21[i] + fZec5[i])))))));
				fZec6[i] = (fRec33[i] + fRec25[i]);
				fRec4[i] = ((fRec12[(i - 1)] + (fRec20[(i - 1)] + (fRec28[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec21[i] + fZec2[i])))))) - (fRec8[(i - 1)] + (fRec16[(i - 1)] + (fRec24[(i - 1)] + (fRec32[(i - 1)] + (fRec9[i] + (fRec17[i] + fZec6[i])))))));
				fRec5[i] = ((fRec8[(i - 1)] + (fRec16[(i - 1)] + (fRec28[(i - 1)] + (fRec36[(i - 1)] + (fRec9[i] + (fRec17[i] + fZec2[i])))))) - (fRec12[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec32[(i - 1)] + (fRec13[i] + (fRec21[i] + fZec6[i])))))));
				fZec7[i] = (fRec37[i] + fRec25[i]);
				fZec8[i] = (fRec33[i] + fRec29[i]);
				fRec6[i] = ((fRec8[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec36[(i - 1)] + (fRec9[i] + (fRec21[i] + fZec7[i])))))) - (fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec13[i] + (fRec17[i] + fZec8[i])))))));
				fRec7[i] = ((fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec24[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec17[i] + fZec7[i])))))) - (fRec8[(i - 1)] + (fRec20[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec9[i] + (fRec21[i] + fZec8[i])))))));
			}
			/* Post code */
			fYec16_idx_save = vsize;
			fYec17_idx_save = vsize;
			for (int j43 = 0; (j43 < 4); j43 = (j43 + 1)) {
				fRec39_perm[j43] = fRec39_tmp[(vsize + j43)];
			}
			for (int j45 = 0; (j45 < 4); j45 = (j45 + 1)) {
				fRec38_perm[j45] = fRec38_tmp[(vsize + j45)];
			}
			for (int j47 = 0; (j47 < 4); j47 = (j47 + 1)) {
				fRec36_perm[j47] = fRec36_tmp[(vsize + j47)];
			}
			fYec14_idx_save = vsize;
			fYec15_idx_save = vsize;
			for (int j37 = 0; (j37 < 4); j37 = (j37 + 1)) {
				fRec35_perm[j37] = fRec35_tmp[(vsize + j37)];
			}
			for (int j39 = 0; (j39 < 4); j39 = (j39 + 1)) {
				fRec34_perm[j39] = fRec34_tmp[(vsize + j39)];
			}
			for (int j41 = 0; (j41 < 4); j41 = (j41 + 1)) {
				fRec32_perm[j41] = fRec32_tmp[(vsize + j41)];
			}
			fYec12_idx_save = vsize;
			fYec13_idx_save = vsize;
			for (int j31 = 0; (j31 < 4); j31 = (j31 + 1)) {
				fRec31_perm[j31] = fRec31_tmp[(vsize + j31)];
			}
			for (int j33 = 0; (j33 < 4); j33 = (j33 + 1)) {
				fRec30_perm[j33] = fRec30_tmp[(vsize + j33)];
			}
			for (int j35 = 0; (j35 < 4); j35 = (j35 + 1)) {
				fRec28_perm[j35] = fRec28_tmp[(vsize + j35)];
			}
			fYec9_idx_save = vsize;
			fYec11_idx_save = vsize;
			for (int j25 = 0; (j25 < 4); j25 = (j25 + 1)) {
				fRec27_perm[j25] = fRec27_tmp[(vsize + j25)];
			}
			for (int j27 = 0; (j27 < 4); j27 = (j27 + 1)) {
				fRec26_perm[j27] = fRec26_tmp[(vsize + j27)];
			}
			for (int j29 = 0; (j29 < 4); j29 = (j29 + 1)) {
				fRec24_perm[j29] = fRec24_tmp[(vsize + j29)];
			}
			fYec7_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec23_perm[j19] = fRec23_tmp[(vsize + j19)];
			}
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec22_perm[j21] = fRec22_tmp[(vsize + j21)];
			}
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				fRec20_perm[j23] = fRec20_tmp[(vsize + j23)];
			}
			fYec5_idx_save = vsize;
			fYec6_idx_save = vsize;
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				fRec19_perm[j13] = fRec19_tmp[(vsize + j13)];
			}
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec18_perm[j15] = fRec18_tmp[(vsize + j15)];
			}
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fRec16_perm[j17] = fRec16_tmp[(vsize + j17)];
			}
			fYec3_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				fRec15_perm[j7] = fRec15_tmp[(vsize + j7)];
			}
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec14_perm[j9] = fRec14_tmp[(vsize + j9)];
			}
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec12_perm[j11] = fRec12_tmp[(vsize + j11)];
			}
			fYec0_idx_save = vsize;
			fYec2_idx_save = vsize;
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec11_perm[j1] = fRec11_tmp[(vsize + j1)];
			}
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec10_perm[j3] = fRec10_tmp[(vsize + j3)];
			}
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec8_perm[j5] = fRec8_tmp[(vsize + j5)];
			}
			for (int j49 = 0; (j49 < 4); j49 = (j49 + 1)) {
				fRec0_perm[j49] = fRec0_tmp[(vsize + j49)];
			}
			for (int j51 = 0; (j51 < 4); j51 = (j51 + 1)) {
				fRec1_perm[j51] = fRec1_tmp[(vsize + j51)];
			}
			for (int j53 = 0; (j53 < 4); j53 = (j53 + 1)) {
				fRec2_perm[j53] = fRec2_tmp[(vsize + j53)];
			}
			for (int j55 = 0; (j55 < 4); j55 = (j55 + 1)) {
				fRec3_perm[j55] = fRec3_tmp[(vsize + j55)];
			}
			for (int j57 = 0; (j57 < 4); j57 = (j57 + 1)) {
				fRec4_perm[j57] = fRec4_tmp[(vsize + j57)];
			}
			for (int j59 = 0; (j59 < 4); j59 = (j59 + 1)) {
				fRec5_perm[j59] = fRec5_tmp[(vsize + j59)];
			}
			for (int j61 = 0; (j61 < 4); j61 = (j61 + 1)) {
				fRec6_perm[j61] = fRec6_tmp[(vsize + j61)];
			}
			for (int j63 = 0; (j63 < 4); j63 = (j63 + 1)) {
				fRec7_perm[j63] = fRec7_tmp[(vsize + j63)];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output1[i] = (FAUSTFLOAT(((fSlow76 * (fRec1[i] - fRec2[i])) + (fSlow77 * float(input1[i])))) + output1[i]);
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = (FAUSTFLOAT(((fSlow76 * (fRec1[i] + fRec2[i])) + (fSlow77 * float(input0[i])))) + output0[i]);
			}
		}
		/* Remaining frames */
		if ((vindex < count)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = (count - vindex);
			/* Vectorizable loop 0 */
			/* Pre code */
			fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 16383);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec10[((i + fYec10_idx) & 16383)] = float(input0[i]);
			}
			/* Post code */
			fYec10_idx_save = vsize;
			/* Vectorizable loop 1 */
			/* Pre code */
			fYec1_idx = ((fYec1_idx + fYec1_idx_save) & 16383);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec1[((i + fYec1_idx) & 16383)] = float(input1[i]);
			}
			/* Post code */
			fYec1_idx_save = vsize;
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec1[i] = (0.300000012f * fYec10[(((i + fYec10_idx) - iSlow11) & 16383)]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec0[i] = (0.300000012f * fYec1[(((i + fYec1_idx) - iSlow11) & 16383)]);
			}
			/* Recursive loop 4 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec11_tmp[j0] = fRec11_perm[j0];
			}
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec10_tmp[j2] = fRec10_perm[j2];
			}
			fYec0_idx = ((fYec0_idx + fYec0_idx_save) & 65535);
			fYec2_idx = ((fYec2_idx + fYec2_idx_save) & 4095);
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec8_tmp[j4] = fRec8_perm[j4];
			}
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				fRec15_tmp[j6] = fRec15_perm[j6];
			}
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec14_tmp[j8] = fRec14_perm[j8];
			}
			fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 65535);
			fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 8191);
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec12_tmp[j10] = fRec12_perm[j10];
			}
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				fRec19_tmp[j12] = fRec19_perm[j12];
			}
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec18_tmp[j14] = fRec18_perm[j14];
			}
			fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 65535);
			fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 8191);
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fRec16_tmp[j16] = fRec16_perm[j16];
			}
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec23_tmp[j18] = fRec23_perm[j18];
			}
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec22_tmp[j20] = fRec22_perm[j20];
			}
			fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 65535);
			fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 8191);
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				fRec20_tmp[j22] = fRec20_perm[j22];
			}
			for (int j24 = 0; (j24 < 4); j24 = (j24 + 1)) {
				fRec27_tmp[j24] = fRec27_perm[j24];
			}
			for (int j26 = 0; (j26 < 4); j26 = (j26 + 1)) {
				fRec26_tmp[j26] = fRec26_perm[j26];
			}
			fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 32767);
			fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 4095);
			for (int j28 = 0; (j28 < 4); j28 = (j28 + 1)) {
				fRec24_tmp[j28] = fRec24_perm[j28];
			}
			for (int j30 = 0; (j30 < 4); j30 = (j30 + 1)) {
				fRec31_tmp[j30] = fRec31_perm[j30];
			}
			for (int j32 = 0; (j32 < 4); j32 = (j32 + 1)) {
				fRec30_tmp[j32] = fRec30_perm[j32];
			}
			fYec12_idx = ((fYec12_idx + fYec12_idx_save) & 32767);
			fYec13_idx = ((fYec13_idx + fYec13_idx_save) & 8191);
			for (int j34 = 0; (j34 < 4); j34 = (j34 + 1)) {
				fRec28_tmp[j34] = fRec28_perm[j34];
			}
			for (int j36 = 0; (j36 < 4); j36 = (j36 + 1)) {
				fRec35_tmp[j36] = fRec35_perm[j36];
			}
			for (int j38 = 0; (j38 < 4); j38 = (j38 + 1)) {
				fRec34_tmp[j38] = fRec34_perm[j38];
			}
			fYec14_idx = ((fYec14_idx + fYec14_idx_save) & 65535);
			fYec15_idx = ((fYec15_idx + fYec15_idx_save) & 8191);
			for (int j40 = 0; (j40 < 4); j40 = (j40 + 1)) {
				fRec32_tmp[j40] = fRec32_perm[j40];
			}
			for (int j42 = 0; (j42 < 4); j42 = (j42 + 1)) {
				fRec39_tmp[j42] = fRec39_perm[j42];
			}
			for (int j44 = 0; (j44 < 4); j44 = (j44 + 1)) {
				fRec38_tmp[j44] = fRec38_perm[j44];
			}
			fYec16_idx = ((fYec16_idx + fYec16_idx_save) & 32767);
			fYec17_idx = ((fYec17_idx + fYec17_idx_save) & 4095);
			for (int j46 = 0; (j46 < 4); j46 = (j46 + 1)) {
				fRec36_tmp[j46] = fRec36_perm[j46];
			}
			for (int j48 = 0; (j48 < 4); j48 = (j48 + 1)) {
				fRec0_tmp[j48] = fRec0_perm[j48];
			}
			for (int j50 = 0; (j50 < 4); j50 = (j50 + 1)) {
				fRec1_tmp[j50] = fRec1_perm[j50];
			}
			for (int j52 = 0; (j52 < 4); j52 = (j52 + 1)) {
				fRec2_tmp[j52] = fRec2_perm[j52];
			}
			for (int j54 = 0; (j54 < 4); j54 = (j54 + 1)) {
				fRec3_tmp[j54] = fRec3_perm[j54];
			}
			for (int j56 = 0; (j56 < 4); j56 = (j56 + 1)) {
				fRec4_tmp[j56] = fRec4_perm[j56];
			}
			for (int j58 = 0; (j58 < 4); j58 = (j58 + 1)) {
				fRec5_tmp[j58] = fRec5_perm[j58];
			}
			for (int j60 = 0; (j60 < 4); j60 = (j60 + 1)) {
				fRec6_tmp[j60] = fRec6_perm[j60];
			}
			for (int j62 = 0; (j62 < 4); j62 = (j62 + 1)) {
				fRec7_tmp[j62] = fRec7_perm[j62];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec11[i] = (fConst2 * ((fRec7[(i - 1)] + fRec7[(i - 2)]) - (fConst3 * fRec11[(i - 1)])));
				fRec10[i] = ((fRec10[(i - 1)] * fSlow7) + ((fSlow1 * (fRec7[(i - 1)] + (fRec11[i] * fSlow9))) * fSlow10));
				fYec0[((i + fYec0_idx) & 65535)] = ((0.353553385f * fRec10[i]) + 9.99999968e-21f);
				fYec2[((i + fYec2_idx) & 4095)] = (((0.600000024f * fRec8[(i - 1)]) + fYec0[(((i + fYec0_idx) - iConst16) & 65535)]) - fZec0[i]);
				fRec8[i] = fYec2[(((i + fYec2_idx) - iConst20) & 4095)];
				fRec9[i] = (0.0f - (0.600000024f * fYec2[((i + fYec2_idx) & 4095)]));
				fRec15[i] = (fConst2 * ((fRec3[(i - 1)] + fRec3[(i - 2)]) - (fConst3 * fRec15[(i - 1)])));
				fRec14[i] = ((fRec14[(i - 1)] * fSlow18) + ((fSlow12 * (fRec3[(i - 1)] + (fRec15[i] * fSlow19))) * fSlow20));
				fYec3[((i + fYec3_idx) & 65535)] = ((0.353553385f * fRec14[i]) + 9.99999968e-21f);
				fYec4[((i + fYec4_idx) & 8191)] = (((0.600000024f * fRec12[(i - 1)]) + fYec3[(((i + fYec3_idx) - iConst31) & 65535)]) - fZec0[i]);
				fRec12[i] = fYec4[(((i + fYec4_idx) - iConst34) & 8191)];
				fRec13[i] = (0.0f - (0.600000024f * fYec4[((i + fYec4_idx) & 8191)]));
				fRec19[i] = (0.0f - (fConst2 * ((fConst3 * fRec19[(i - 1)]) - (fRec5[(i - 1)] + fRec5[(i - 2)]))));
				fRec18[i] = ((fRec18[(i - 1)] * fSlow27) + ((fSlow21 * (fRec5[(i - 1)] + (fRec19[i] * fSlow28))) * fSlow29));
				fYec5[((i + fYec5_idx) & 65535)] = ((0.353553385f * fRec18[i]) + 9.99999968e-21f);
				fYec6[((i + fYec6_idx) & 8191)] = (fYec5[(((i + fYec5_idx) - iConst45) & 65535)] + (fZec0[i] + (0.600000024f * fRec16[(i - 1)])));
				fRec16[i] = fYec6[(((i + fYec6_idx) - iConst48) & 8191)];
				fRec17[i] = (0.0f - (0.600000024f * fYec6[((i + fYec6_idx) & 8191)]));
				fRec23[i] = (fConst2 * ((fRec1[(i - 1)] + fRec1[(i - 2)]) - (fConst3 * fRec23[(i - 1)])));
				fRec22[i] = ((fRec22[(i - 1)] * fSlow36) + ((fSlow30 * (fRec1[(i - 1)] + (fRec23[i] * fSlow37))) * fSlow38));
				fYec7[((i + fYec7_idx) & 65535)] = ((0.353553385f * fRec22[i]) + 9.99999968e-21f);
				fYec8[((i + fYec8_idx) & 8191)] = (fYec7[(((i + fYec7_idx) - iConst59) & 65535)] + ((0.600000024f * fRec20[(i - 1)]) + fZec0[i]));
				fRec20[i] = fYec8[(((i + fYec8_idx) - iConst62) & 8191)];
				fRec21[i] = (0.0f - (0.600000024f * fYec8[((i + fYec8_idx) & 8191)]));
				fRec27[i] = (fConst2 * ((fRec6[(i - 1)] + fRec6[(i - 2)]) - (fConst3 * fRec27[(i - 1)])));
				fRec26[i] = ((fRec26[(i - 1)] * fSlow45) + ((fSlow39 * (fRec6[(i - 1)] + (fRec27[i] * fSlow46))) * fSlow47));
				fYec9[((i + fYec9_idx) & 32767)] = ((0.353553385f * fRec26[i]) + 9.99999968e-21f);
				fYec11[((i + fYec11_idx) & 4095)] = (fYec9[(((i + fYec9_idx) - iConst73) & 32767)] - (fZec1[i] + (0.600000024f * fRec24[(i - 1)])));
				fRec24[i] = fYec11[(((i + fYec11_idx) - iConst76) & 4095)];
				fRec25[i] = (0.600000024f * fYec11[((i + fYec11_idx) & 4095)]);
				fRec31[i] = (0.0f - (fConst2 * ((fConst3 * fRec31[(i - 1)]) - (fRec2[(i - 1)] + fRec2[(i - 2)]))));
				fRec30[i] = ((fRec30[(i - 1)] * fSlow54) + ((fSlow48 * (fRec2[(i - 1)] + (fRec31[i] * fSlow55))) * fSlow56));
				fYec12[((i + fYec12_idx) & 32767)] = ((0.353553385f * fRec30[i]) + 9.99999968e-21f);
				fYec13[((i + fYec13_idx) & 8191)] = (fYec12[(((i + fYec12_idx) - iConst87) & 32767)] - (fZec1[i] + (0.600000024f * fRec28[(i - 1)])));
				fRec28[i] = fYec13[(((i + fYec13_idx) - iConst90) & 8191)];
				fRec29[i] = (0.600000024f * fYec13[((i + fYec13_idx) & 8191)]);
				fRec35[i] = (fConst2 * ((fRec4[(i - 1)] + fRec4[(i - 2)]) - (fConst3 * fRec35[(i - 1)])));
				fRec34[i] = ((fRec34[(i - 1)] * fSlow63) + ((fSlow57 * (fRec4[(i - 1)] + (fRec35[i] * fSlow64))) * fSlow65));
				fYec14[((i + fYec14_idx) & 65535)] = ((0.353553385f * fRec34[i]) + 9.99999968e-21f);
				fYec15[((i + fYec15_idx) & 8191)] = ((fZec1[i] + fYec14[(((i + fYec14_idx) - iConst101) & 65535)]) - (0.600000024f * fRec32[(i - 1)]));
				fRec32[i] = fYec15[(((i + fYec15_idx) - iConst104) & 8191)];
				fRec33[i] = (0.600000024f * fYec15[((i + fYec15_idx) & 8191)]);
				fRec39[i] = (fConst2 * ((fRec0[(i - 1)] + fRec0[(i - 2)]) - (fConst3 * fRec39[(i - 1)])));
				fRec38[i] = ((fRec38[(i - 1)] * fSlow72) + ((fSlow66 * (fRec0[(i - 1)] + (fRec39[i] * fSlow73))) * fSlow74));
				fYec16[((i + fYec16_idx) & 32767)] = ((0.353553385f * fRec38[i]) + 9.99999968e-21f);
				fYec17[((i + fYec17_idx) & 4095)] = ((fYec16[(((i + fYec16_idx) - iConst115) & 32767)] + fZec1[i]) - (0.600000024f * fRec36[(i - 1)]));
				fRec36[i] = fYec17[(((i + fYec17_idx) - iConst118) & 4095)];
				fRec37[i] = (0.600000024f * fYec17[((i + fYec17_idx) & 4095)]);
				fZec2[i] = (fRec37[i] + fRec29[i]);
				fZec3[i] = (fRec25[i] + (fRec33[i] + fZec2[i]));
				fRec0[i] = (fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec9[i] + (fRec17[i] + (fRec21[i] + fZec3[i]))))))))))));
				fRec1[i] = ((fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + fZec3[i])))) - (fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec13[i] + (fRec9[i] + (fRec21[i] + fRec17[i]))))))));
				fZec4[i] = (fRec37[i] + fRec33[i]);
				fZec5[i] = (fRec29[i] + fRec25[i]);
				fRec2[i] = ((fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + (fRec17[i] + (fRec21[i] + fZec4[i])))))) - (fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec13[i] + (fRec9[i] + fZec5[i])))))));
				fRec3[i] = ((fRec8[(i - 1)] + (fRec12[(i - 1)] + (fRec32[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec9[i] + fZec4[i])))))) - (fRec16[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec28[(i - 1)] + (fRec17[i] + (fRec21[i] + fZec5[i])))))));
				fZec6[i] = (fRec33[i] + fRec25[i]);
				fRec4[i] = ((fRec12[(i - 1)] + (fRec20[(i - 1)] + (fRec28[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec21[i] + fZec2[i])))))) - (fRec8[(i - 1)] + (fRec16[(i - 1)] + (fRec24[(i - 1)] + (fRec32[(i - 1)] + (fRec9[i] + (fRec17[i] + fZec6[i])))))));
				fRec5[i] = ((fRec8[(i - 1)] + (fRec16[(i - 1)] + (fRec28[(i - 1)] + (fRec36[(i - 1)] + (fRec9[i] + (fRec17[i] + fZec2[i])))))) - (fRec12[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec32[(i - 1)] + (fRec13[i] + (fRec21[i] + fZec6[i])))))));
				fZec7[i] = (fRec37[i] + fRec25[i]);
				fZec8[i] = (fRec33[i] + fRec29[i]);
				fRec6[i] = ((fRec8[(i - 1)] + (fRec20[(i - 1)] + (fRec24[(i - 1)] + (fRec36[(i - 1)] + (fRec9[i] + (fRec21[i] + fZec7[i])))))) - (fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec13[i] + (fRec17[i] + fZec8[i])))))));
				fRec7[i] = ((fRec12[(i - 1)] + (fRec16[(i - 1)] + (fRec24[(i - 1)] + (fRec36[(i - 1)] + (fRec13[i] + (fRec17[i] + fZec7[i])))))) - (fRec8[(i - 1)] + (fRec20[(i - 1)] + (fRec28[(i - 1)] + (fRec32[(i - 1)] + (fRec9[i] + (fRec21[i] + fZec8[i])))))));
			}
			/* Post code */
			fYec16_idx_save = vsize;
			fYec17_idx_save = vsize;
			for (int j43 = 0; (j43 < 4); j43 = (j43 + 1)) {
				fRec39_perm[j43] = fRec39_tmp[(vsize + j43)];
			}
			for (int j45 = 0; (j45 < 4); j45 = (j45 + 1)) {
				fRec38_perm[j45] = fRec38_tmp[(vsize + j45)];
			}
			for (int j47 = 0; (j47 < 4); j47 = (j47 + 1)) {
				fRec36_perm[j47] = fRec36_tmp[(vsize + j47)];
			}
			fYec14_idx_save = vsize;
			fYec15_idx_save = vsize;
			for (int j37 = 0; (j37 < 4); j37 = (j37 + 1)) {
				fRec35_perm[j37] = fRec35_tmp[(vsize + j37)];
			}
			for (int j39 = 0; (j39 < 4); j39 = (j39 + 1)) {
				fRec34_perm[j39] = fRec34_tmp[(vsize + j39)];
			}
			for (int j41 = 0; (j41 < 4); j41 = (j41 + 1)) {
				fRec32_perm[j41] = fRec32_tmp[(vsize + j41)];
			}
			fYec12_idx_save = vsize;
			fYec13_idx_save = vsize;
			for (int j31 = 0; (j31 < 4); j31 = (j31 + 1)) {
				fRec31_perm[j31] = fRec31_tmp[(vsize + j31)];
			}
			for (int j33 = 0; (j33 < 4); j33 = (j33 + 1)) {
				fRec30_perm[j33] = fRec30_tmp[(vsize + j33)];
			}
			for (int j35 = 0; (j35 < 4); j35 = (j35 + 1)) {
				fRec28_perm[j35] = fRec28_tmp[(vsize + j35)];
			}
			fYec9_idx_save = vsize;
			fYec11_idx_save = vsize;
			for (int j25 = 0; (j25 < 4); j25 = (j25 + 1)) {
				fRec27_perm[j25] = fRec27_tmp[(vsize + j25)];
			}
			for (int j27 = 0; (j27 < 4); j27 = (j27 + 1)) {
				fRec26_perm[j27] = fRec26_tmp[(vsize + j27)];
			}
			for (int j29 = 0; (j29 < 4); j29 = (j29 + 1)) {
				fRec24_perm[j29] = fRec24_tmp[(vsize + j29)];
			}
			fYec7_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec23_perm[j19] = fRec23_tmp[(vsize + j19)];
			}
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec22_perm[j21] = fRec22_tmp[(vsize + j21)];
			}
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				fRec20_perm[j23] = fRec20_tmp[(vsize + j23)];
			}
			fYec5_idx_save = vsize;
			fYec6_idx_save = vsize;
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				fRec19_perm[j13] = fRec19_tmp[(vsize + j13)];
			}
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec18_perm[j15] = fRec18_tmp[(vsize + j15)];
			}
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fRec16_perm[j17] = fRec16_tmp[(vsize + j17)];
			}
			fYec3_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				fRec15_perm[j7] = fRec15_tmp[(vsize + j7)];
			}
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec14_perm[j9] = fRec14_tmp[(vsize + j9)];
			}
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec12_perm[j11] = fRec12_tmp[(vsize + j11)];
			}
			fYec0_idx_save = vsize;
			fYec2_idx_save = vsize;
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec11_perm[j1] = fRec11_tmp[(vsize + j1)];
			}
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec10_perm[j3] = fRec10_tmp[(vsize + j3)];
			}
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec8_perm[j5] = fRec8_tmp[(vsize + j5)];
			}
			for (int j49 = 0; (j49 < 4); j49 = (j49 + 1)) {
				fRec0_perm[j49] = fRec0_tmp[(vsize + j49)];
			}
			for (int j51 = 0; (j51 < 4); j51 = (j51 + 1)) {
				fRec1_perm[j51] = fRec1_tmp[(vsize + j51)];
			}
			for (int j53 = 0; (j53 < 4); j53 = (j53 + 1)) {
				fRec2_perm[j53] = fRec2_tmp[(vsize + j53)];
			}
			for (int j55 = 0; (j55 < 4); j55 = (j55 + 1)) {
				fRec3_perm[j55] = fRec3_tmp[(vsize + j55)];
			}
			for (int j57 = 0; (j57 < 4); j57 = (j57 + 1)) {
				fRec4_perm[j57] = fRec4_tmp[(vsize + j57)];
			}
			for (int j59 = 0; (j59 < 4); j59 = (j59 + 1)) {
				fRec5_perm[j59] = fRec5_tmp[(vsize + j59)];
			}
			for (int j61 = 0; (j61 < 4); j61 = (j61 + 1)) {
				fRec6_perm[j61] = fRec6_tmp[(vsize + j61)];
			}
			for (int j63 = 0; (j63 < 4); j63 = (j63 + 1)) {
				fRec7_perm[j63] = fRec7_tmp[(vsize + j63)];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output1[i] = (FAUSTFLOAT(((fSlow76 * (fRec1[i] - fRec2[i])) + (fSlow77 * float(input1[i])))) + output1[i]);
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = (FAUSTFLOAT(((fSlow76 * (fRec1[i] + fRec2[i])) + (fSlow77 * float(input0[i])))) + output0[i]);
			}
		}
	}

};

struct RegisterVAEReverb {
	static EffectBase* create() {
		auto ptr = new VAEReverb();
		VAE_PROFILER_MALLOC_L(ptr, sizeof(VAEReverb), vae::core::profiler::dspAllocator)
		return ptr;
	}
	RegisterVAEReverb() {
		EffectsList::instance().add("VAEReverb", &create);
	}
} __registerVAEReverb;

} } } // vae::core::effect

#endif
