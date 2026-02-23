#ifndef CORE_MATH_MATRIX3_HPP
#define CORE_MATH_MATRIX3_HPP

#include "./../types.hpp"
#include "./../vector/vector3.hpp"

#pragma warning(push)
#pragma warning(disable: 4201)
// IMPORTANT Column-major
struct Matrix3 {
	union {
		f32 data[9];
		struct {
			f32 n11, n21, n31; // col 1
			f32 n12, n22, n32; // col 2
			f32 n13, n23, n33; // col 3
		};
		struct {
			Vector3 col1;
			Vector3 col2;
			Vector3 col3;
		};
		struct {
			Vector3 x;
			Vector3 y;
			Vector3 z;
		};
		struct {
			Vector3 i;
			Vector3 j;
			Vector3 k;
		};
		struct {
			Vector3 x_axis;
			Vector3 y_axis;
			Vector3 z_axis;
		};
		struct {
			Vector3 right;
			Vector3 up;
			Vector3 forward;
		};
	};
};
#pragma warning(pop)

constexpr Vector3 operator*(const Matrix3& A, const Vector3& x) {
	return x.x * A.col1 + x.y * A.col2 + x.z * A.col3;
}

constexpr Matrix3 operator*(const Matrix3& a, const Matrix3& b) {
	Matrix3 r;

	r.n11 = a.n11 * b.n11 + a.n12 * b.n21 + a.n13 * b.n31;
	r.n21 = a.n21 * b.n11 + a.n22 * b.n21 + a.n23 * b.n31;
	r.n31 = a.n31 * b.n11 + a.n32 * b.n21 + a.n33 * b.n31;

	r.n12 = a.n11 * b.n12 + a.n12 * b.n22 + a.n13 * b.n32;
	r.n22 = a.n21 * b.n12 + a.n22 * b.n22 + a.n23 * b.n32;
	r.n32 = a.n31 * b.n12 + a.n32 * b.n22 + a.n33 * b.n32;

	r.n13 = a.n11 * b.n13 + a.n12 * b.n23 + a.n13 * b.n33;
	r.n23 = a.n21 * b.n13 + a.n22 * b.n23 + a.n23 * b.n33;
	r.n33 = a.n31 * b.n13 + a.n32 * b.n23 + a.n33 * b.n33;

	return r;
}

namespace Math {

	constexpr void transpose(Matrix3* in, Matrix3* out) {
		Matrix3& m = *in;
		Matrix3& r = *out;

		r.n11 = m.n11; r.n12 = m.n21; r.n13 = m.n31;
		r.n21 = m.n12; r.n22 = m.n22; r.n23 = m.n32;
		r.n31 = m.n13; r.n32 = m.n23; r.n33 = m.n33;
	}

	constexpr f32 determinant(Matrix3* in) {
		Matrix3& m = *in;
		return
			m.n11 * (m.n22 * m.n33 - m.n23 * m.n32)
			- m.n12 * (m.n21 * m.n33 - m.n23 * m.n31)
			+ m.n13 * (m.n21 * m.n32 - m.n22 * m.n31);
	}

	constexpr void inverse(Matrix3* in, Matrix3* out) {
		f32 det = determinant(in);
		f32 inv_det = 1.0f / det;

		Matrix3& m = *in;
		Matrix3& r = *out;

		r.n11 = (m.n22 * m.n33 - m.n23 * m.n32) * inv_det;
		r.n12 = -(m.n12 * m.n33 - m.n13 * m.n32) * inv_det;
		r.n13 = (m.n12 * m.n23 - m.n13 * m.n22) * inv_det;

		r.n21 = -(m.n21 * m.n33 - m.n23 * m.n31) * inv_det;
		r.n22 = (m.n11 * m.n33 - m.n13 * m.n31) * inv_det;
		r.n23 = -(m.n11 * m.n23 - m.n13 * m.n21) * inv_det;

		r.n31 = (m.n21 * m.n32 - m.n22 * m.n31) * inv_det;
		r.n32 = -(m.n11 * m.n32 - m.n12 * m.n31) * inv_det;
		r.n33 = (m.n11 * m.n22 - m.n12 * m.n21) * inv_det;
	}

} // namespace Math

#endif // CORE_MATH_MATRIX3_HPP
