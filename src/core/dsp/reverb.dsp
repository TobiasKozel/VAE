import("stdfaust.lib");

// params are alphabetically sorted in the generated dsp
mix = vslider( "mix", 1.0, 0, 1, 0.01);
fb1 = vslider( "fb1", 0.5, 0, 1, 0.01);
fb2 = vslider( "fb2", 0.5, 0, 1, 0.01);
pre = vslider( "pre", 0.5, 0, 1, 0.01);

revMix = _, _ : re.zita_rev1_stereo(pre * 100, 500, 3000, fb1 * 10, fb2 * 10, ma.SR) : _ * mix, _ * mix;
process = _, _ <: revMix, (_ * (1 - mix), _ * (1 - mix)) :> _, _;
