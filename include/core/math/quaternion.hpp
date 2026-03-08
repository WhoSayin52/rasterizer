#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "./types.hpp"
#include "./vector/vector3.hpp"
#include "./matrix/matrix4.hpp"

#include "cmath"

#pragma warning(push)
#pragma warning(disable: 4201)
struct Quaternion {
	union {
		f32 data[4];
		struct {
			f32 x, y, z, w;
		};
	};
};
#pragma warning(pop)

constexpr Quaternion operator*(Quaternion a, Quaternion b) {

	f32 real = a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z);
	f32 i = a.w * b.x + b.w * a.x + (a.y * b.z - a.z * b.y);
	f32 j = a.w * b.y + b.w * a.y + (a.z * b.x - a.x * b.z);
	f32 k = a.w * b.z + b.w * a.z + (a.x * b.y - a.y * b.x);

	return Quaternion{ i, j, k, real };
}

namespace Math {

	inline f32 length(Quaternion q) {
		return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	}

	inline Quaternion normalize(Quaternion q) {
		f32 magnitude = length(q);
		assert(magnitude > 0.0f);
		return Quaternion{ (q.x / magnitude), (q.y / magnitude), (q.z / magnitude), (q.w / magnitude) };
	}

	inline Quaternion conjugate(Quaternion q) {
		return Quaternion{ -q.x, -q.y, -q.z, q.w };
	}

	inline Quaternion inverse(Quaternion q) {
		f32 length_squared = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
		assert(length_squared > 0);
		return Quaternion{ (-q.x / length_squared), (-q.y / length_squared), (-q.z / length_squared), (q.w / length_squared) };
	}

	inline Quaternion create_quaternion(f32 angle, Vector3 axis) {
		f32 EPS = 1e-5f;
		assert(fabs(length(axis) - 1.0f) < EPS);

		f32 half_angle = angle * 0.5f;

		f32 s = sinf(half_angle);
		f32 w = cosf(half_angle);

		return Quaternion{ s * axis.x, s * axis.y, s * axis.z, w };
	}

	inline Vector3 rotate(Vector3 v, Quaternion q) {
		f32 EPS = 1e-5f;
		assert(fabs(length(q) - 1.0f) < EPS);

		Quaternion p = { v.x, v.y, v.z, 0 };
		p = q * p * conjugate(q);

		assert(fabs(p.w) < EPS);

		return Vector3{ p.x, p.y, p.z };
	}

	constexpr Matrix4 get_rotation_matrix4(Quaternion q) {

		f32 xx = q.x * q.x;
		f32 yy = q.y * q.y;
		f32 zz = q.z * q.z;
		f32 xy = q.x * q.y;
		f32 xz = q.x * q.z;
		f32 yz = q.y * q.z;
		f32 wx = q.w * q.x;
		f32 wy = q.w * q.y;
		f32 wz = q.w * q.z;

		Matrix4 result;
		result.n11 = 1 - 2 * (yy + zz); result.n12 = 2 * (xy - wz); result.n13 = 2 * (xz + wy); result.n14 = 0;
		result.n21 = 2 * (xy + wz); result.n22 = 1 - 2 * (xx + zz); result.n23 = 2 * (yz - wx); result.n24 = 0;
		result.n31 = 2 * (xz - wy); result.n32 = 2 * (yz + wx); result.n33 = 1 - 2 * (xx + yy); result.n34 = 0;
		result.n41 = 0; result.n42 = 0; result.n43 = 0; result.n44 = 1;

		return result;
	}

} // namespace Math


#endif // QUATERNION_HPP
