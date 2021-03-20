#include "../../util/TTimer.hpp"

#ifndef ITERATIONS
	#define ITERATIONS 1000
#endif

#ifdef TKLB_NO_SIMD
	#ifdef TKLB_SAMPLE_FLOAT
		#define TIMER(unit) SectionTimer timer(__FILE__ "\tNo SIMD\tfloat\t", SectionTimer::unit, ITERATIONS)
	#else
		#define TIMER(unit) SectionTimer timer(__FILE__ "\tNo SIMD\tdouble\t", SectionTimer::unit, ITERATIONS)
	#endif
#else
	#ifdef TKLB_SAMPLE_FLOAT
		#define TIMER(unit) SectionTimer timer(__FILE__ "\tSIMD\tfloat\t", SectionTimer::unit, ITERATIONS)
	#else
		#define TIMER(unit) SectionTimer timer(__FILE__ "\tSIMD\tdouble\t", SectionTimer::unit, ITERATIONS)
	#endif
#endif

using namespace tklb;
using namespace std;

