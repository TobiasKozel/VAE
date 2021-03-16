#include <cmath>
#include "../util/TTimer.h"
#include "../util/TLeakChecker.h"

using namespace tklb;
using namespace std;

int ret;
#define returnNonZero(val) ret = val; if(ret != 0) { return ret; }

#define memcheck() 	if (allocationCount != 0) { return 100; } \
					else if (curruptions != 0) { return 101; }

bool close(float a, float b, float epsylon = 0.01) {
	return std::abs(a - b) < epsylon;
}
