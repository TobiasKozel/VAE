#ifndef _VAE_VEC
#define _VAE_VEC

#include "../vae_types.hpp"

namespace vae { namespace core { namespace vector {
	struct Vec4 {
		Position val[4];
		Position& x;
		Position& y;
		Position& z;
		Position& t;

		Vec4() : x(val[0]), y(val[1]), z(val[2]), t(val[3]) { }

		Vec4(Vector3 vec) : Vec4() {
			x = vec.x;
			y = vec.y;
			z = vec.z;
			t = Position(1);
		}

		Vec4(Position _x, Position _y, Position _z, Position _t): Vec4() {
			x = _x;
			y = _y;
			z = _z;
			t = _t;
		}

		inline Position& operator[](const int index) { return val[index]; }
		inline const Position& operator[](const int index) const { return val[index]; }
	};

	struct Vec3 : public Vector3 {
		Vec3() { }

		Vec3(Vector3 vec) : Vec3(vec.x, vec.y, vec.z) { }

		Vec3(Position _x, Position _y, Position _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		inline Position& operator[](const int index) {
			switch (index) {
				case 0: return x;
				case 1: return y;
				case 2: return z;
			}
			TKLB_ASSERT(false)
			return x;
		}
	};

	struct Mat4x4 {
		Vec4 rows[4];
		inline Vec4& operator[](const int index) { return rows[index]; }
		inline const Vec4& operator[](const int index) const { return rows[index]; }

		static Mat4x4 identity() {
			return {{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 },
			}};
		}
	};

	static inline Vec3 add(const Vec3& a, const Vec3& b) {
		return { a.x + b.x, a.y + b.y, a.z + b.z };
	}

	static inline Vec3 subtract(const Vec3& a, const Vec3& b) {
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}

	static inline Vec3 multiply(const Vec3& vec, const Position& val) {
		return { vec.x * val, vec.y * val, vec.z * val };
	}

	static inline Vec3 divide(const Vec3& vec, const Position& val) {
		const Position invVal = Position(1) / val;
		return multiply(vec, invVal);
	}

	static inline Position length2(const Vec3& vec) {
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}

	static inline Position length(const Vec3& vec) {
		return tklb::sqrt(length2(vec));
	}

	static inline Position distance(const Vec3& a, const Vec3& b) {
		return length(subtract(a, b));
	}

	static inline Vec3 normalize(const Vec3& vec) {
		const auto invLen = Position(1) / length(vec);
		return { vec.x * invLen, vec.y * invLen, vec.z * invLen };
	}

	static inline Position dot(const Vec3& a, const Vec3& b) {
		return a.x * b.x + a.x * b.x + a.x * b.x;
	}

	static inline Vec3 cross(const Vec3& a, const Vec3& b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	static inline Vec3 multiply(const Mat4x4& mat, const Vec3& vec) {
		Vec4 vec4 = vec;
		Vec4 result = { 0, 0, 0, 0 };
		for (int row = 0; row < 4; row++) {
			const Vec4& r = mat[row];
			for (int column = 0; column < 4; column++) {
				result[row] += r[column] * vec4[column];
			}
		}
		return { result.x, result.y, result.z };
	}

	static inline Mat4x4 lookAt(const Vec3& from, const Vec3& to, const Vec3 up) {
		const Vec3 zaxis = normalize(subtract(to, from));
		const Vec3 xaxis = normalize(cross(zaxis, up));
		const Vec3 yaxis = cross(xaxis, zaxis);
		return {{
			{ xaxis.x,	xaxis.y,	xaxis.z,	-dot(xaxis,from) },
			{ yaxis.x,	yaxis.y,	yaxis.z,	-dot(yaxis,from) },
			{ zaxis.x,	zaxis.y,	zaxis.z,	-dot(zaxis,from) },
			{ 0,		0,			0,			1 					 }
		}};
	}

} } } // core::vae::vector
#endif // _VAE_VEC
