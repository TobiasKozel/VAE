#ifndef TKLB_MATH
#define TKLB_MATH

#include <algorithm>

namespace tklb {

	constexpr double PI = 3.14159265358979323846;

	template<typename T>
	inline T powerOf2(const T& v) {
		T p = 1;
		while (p < v) { p *= 2; }
		return p;
	}

	template <typename T>
	inline T clamp(const T v, const T min, const T max) {
		return std::min(max, std::max(v, min));
	}
} // namespace

#endif // TKLB_MATH
