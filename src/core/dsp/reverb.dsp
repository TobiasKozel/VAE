import("stdfaust.lib");

// params are alphabetically sorted in the generated dsp
mix = vslider( "mix", 1.0, 0, 1, 0.01) : si.smooth(0.999) : si.smooth(0.999), 0 : max, 1 : min;
fb1 = vslider( "fb1", 0.5, 0, 1, 0.01) : si.smooth(0.999) : si.smooth(0.999), 0.02 : max, 0.97 : min;
fb2 = vslider( "fb2", 0.5, 0, 1, 0.01) : si.smooth(0.999) : si.smooth(0.999), 0.02 : max, 0.97 : min;
pre = vslider( "pre", 0.5, 0, 1, 0.01) : si.smooth(0.999) : si.smooth(0.999), 0.02 : max, 0.97 : min;

revMix = _, _ : re.zita_rev1_stereo(pre * 100, 500, 3000, fb1 * 10, fb2 * 10, ma.SR) : _ * mix, _ * mix;
process = _, _ <: revMix, (_ * (1 - mix), _ * (1 - mix)) :> _, _;
