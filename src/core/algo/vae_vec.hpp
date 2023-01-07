#ifndef _VAE_VEC
#define _VAE_VEC

#include "../../../include/vae/vae.hpp"
#include "../../wrapped/vae_tklb.hpp"

namespace vae { namespace core { namespace vector {
	struct Vec4 {
		Position x = 0, y = 0, z = 0, t = 0;

		static constexpr Vec4 from(const Vector3& vec) {
			return { vec.x, vec.y, vec.z, Position(1) };
		}

		constexpr Vector3 vec3() { return { x, y, z }; }

		constexpr Position& operator[](const int index) {
			TKLB_ASSERT(index < 4)
			switch (index) {
				case 0: return x; case 1: return y; case 2: return z; default: return t;
			}
		}
		constexpr const Position& operator[](const int index) const {
			TKLB_ASSERT(index < 4)
			switch (index) {
				case 0: return x; case 1: return y; case 2: return z; default: return t;
			}
		}
	};

	struct Mat4x4 {
		Vec4 rows[4];
		inline Vec4& operator[](const int index) { return rows[index]; }
		inline const Vec4& operator[](const int index) const { return rows[index]; }

		static Mat4x4 identity() {
			return { {
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 },
			} };
		}
	};

	constexpr Vector3 add(const Vector3& a, const Vector3& b) {
		return { a.x + b.x, a.y + b.y, a.z + b.z };
	}

	constexpr Vector3 subtract(const Vector3& a, const Vector3& b) {
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}

	constexpr Vector3 multiply(const Vector3& vec, const Position& val) {
		return { vec.x * val, vec.y * val, vec.z * val };
	}

	constexpr Vector3 divide(const Vector3& vec, const Position& val) {
		return multiply(vec, Position(1) / val);
	}

	constexpr Position length2(const Vector3& vec) {
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}

	constexpr Position length(const Vector3& vec) {
		return tklb::sqrt(length2(vec));
	}

	constexpr Position distance(const Vector3& a, const Vector3& b) {
		return length(subtract(a, b));
	}

	constexpr Vector3 normalize(const Vector3& vec) {
		return multiply(vec, Position(1) / length(vec));
	}

	constexpr Position dot(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	constexpr Vector3 cross(const Vector3& a, const Vector3& b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	constexpr Vector3 multiply(const Mat4x4& mat, const Vector3& vec) {
		const auto vec4 = Vec4::from(vec);
		Vec4 result;
		for (int row = 0; row < 4; row++) {
			const Vec4& r = mat[row];
			for (int column = 0; column < 4; column++) {
				result[row] += r[column] * vec4[column];
			}
		}
		return result.vec3();
	}

	constexpr Mat4x4 lookAt(const Vector3& from, const Vector3& to, const Vector3 up) {
		const auto zaxis = normalize(subtract(to, from));
		const auto xaxis = normalize(cross(zaxis, up));
		const auto yaxis = cross(xaxis, zaxis);
		return {{
			{ xaxis.x,	xaxis.y,	xaxis.z,	-dot(xaxis,from) },
			{ yaxis.x,	yaxis.y,	yaxis.z,	-dot(yaxis,from) },
			{ zaxis.x,	zaxis.y,	zaxis.z,	-dot(zaxis,from) },
			{ 0,		0,			0,			1 					  }
		}};
	}

} } } // core::vae::vector
#endif // _VAE_VEC
