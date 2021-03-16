#ifndef TKLB_FFT
#define TKLB_FFT

#ifdef TKLB_USE_OOURA
	#include "./TOouraFFT.h"
namespace tklb {
	using FFT = FFTOoura;
}
#else
	#include "./Tpffft.h"
namespace tklb {
	using FFT = FFTpffft;
}
#endif

#endif // TKLB_FFT
