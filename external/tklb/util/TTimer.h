#ifndef TKLB_TIMER
#define TKLB_TIMER

#include <string>
#include <chrono>
#include <ctime>
#include "./TPrint.h"

namespace tklb {

class SectionTimer {
public:
	enum Unit {
		Miliseconds,
		Microseconds,
		Nanoseconds
	} mUnit;

private:
	const char* mMessage;

	unsigned int mDivider;

	#ifdef _MSC_VER
		using Time = std::chrono::time_point<std::chrono::steady_clock>;
	#else
		using Time = std::chrono::system_clock::time_point;
	#endif

	Time mStart;

public:
	SectionTimer(const char* message = "", Unit unit = Microseconds, unsigned int divider = 1) {
		mMessage = message;
		mUnit = unit;
		mStart = std::chrono::high_resolution_clock::now();
		mDivider = divider;
	}

	~SectionTimer() {
		Time now = std::chrono::high_resolution_clock::now();
		if(mUnit == Nanoseconds) {
			auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(now - mStart);
			TKLB_PRINT("%s\t%i\tnanoseconds\n", mMessage, int(dur.count() / mDivider))
		}
		if (mUnit == Microseconds) {
			auto dur = std::chrono::duration_cast<std::chrono::microseconds>(now - mStart);
			TKLB_PRINT("%s\t%i\tmicroseconds\n", mMessage, int(dur.count() / mDivider))
		}
		if (mUnit == Miliseconds) {
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - mStart);
			TKLB_PRINT("%s\t%i\tmilliseconds\n", mMessage, int(dur.count() / mDivider))
		}
	}
};

class SectionClock {
	const char* mMessage;
	std::clock_t mStart;

public:
	SectionClock(const char* message = "") {
		mMessage = message;
		mStart = std::clock();
	}

	~SectionClock() {
		std::clock_t now = std::clock();
		TKLB_PRINT("%s\t%i\tCycles\n", mMessage, int(now - mStart))
	}
};

} // namespace

#endif // TKLB_TIMER
