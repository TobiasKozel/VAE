#ifndef _VAE_CONVOLVE
#define _VAE_CONVOLVE

namespace vae { namespace core {
	struct Convolve {
		static void convolve() {

			// TODO make the frequency domainversion work
			// for (int c = 0; c < 1; c++) {
			// 	mTimeDomain.set(0);
			// 	mTimeDomain.set(signal, remaining, v.time);
			// 	mTimeDomain.setValidSize(mTimeDomain.size());
			// 	mFFT.forward(mTimeDomain, mFrequencyDomain);

			// 	auto re = mFrequencyDomain[0];
			// 	auto im = mFrequencyDomain[1];
			// 	const auto reA = ir[c][0];
			// 	const auto imA = ir[c][1];
			// 	for (Size i = 0; i < mFrequencyDomain.size(); i++) {
			// 		const auto _reB = re[i];
			// 		const auto _imB = im[i];
			// 		re[i] += reA[i] * _reB - imA[i] * _imB;
			// 		im[i] += reA[i] * _imB + imA[i] * _reB;
			// 	}

			// 	mFFT.back(mFrequencyDomain, mTimeDomain);
			// 	for (Size i = 0; i < remaining; i++) {
			// 		target[c][i] += mTimeDomain[0][i];
			// 	}
			// }
		}
	};
} } // core::vae

#endif // _VAE_CONVOLVE
