#ifndef CORE_MATH_MATRIX4_HPP
#define CORE_MATH_MATRIX4_HPP

#include "./../types.hpp"
#include "./../vector/vector4.hpp"

#pragma warning(push)
#pragma warning(disable: 4201)
// IMPORTANT Column-major
struct Matrix4 {
	union {
		f32 data[16];
		struct {
			f32 n11, n21, n31, n41; // col 1
			f32 n12, n22, n32, n42; // col 2
			f32 n13, n23, n33, n43; // col 3
			f32 n14, n24, n34, n44; // col 4
		};
		struct {
			Vector4 col1;
			Vector4 col2;
			Vector4 col3;
			Vector4 col4;
		};
		struct {
			Vector4 x;
			Vector4 y;
			Vector4 z;
			Vector4 w;
		};
		struct {
			Vector4 i;
			Vector4 j;
			Vector4 k;
			Vector4 l;
		};
		struct {
			Vector4 x_axis;
			Vector4 y_axis;
			Vector4 z_axis;
			Vector4 w_axis;
		};
		// Common 3D transform naming
		struct {
			Vector4 right;
			Vector4 up;
			Vector4 forward;
			Vector4 position;
		};
	};
};
#pragma warning(pop)

constexpr Vector4 operator*(const Matrix4& A, const Vector4& x) {
	return x.x * A.col1 + x.y * A.col2 + x.z * A.col3 + x.w * A.col4;
}

constexpr Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
	Matrix4 r;

	r.n11 = a.n11 * b.n11 + a.n12 * b.n21 + a.n13 * b.n31 + a.n14 * b.n41;
	r.n21 = a.n21 * b.n11 + a.n22 * b.n21 + a.n23 * b.n31 + a.n24 * b.n41;
	r.n31 = a.n31 * b.n11 + a.n32 * b.n21 + a.n33 * b.n31 + a.n34 * b.n41;
	r.n41 = a.n41 * b.n11 + a.n42 * b.n21 + a.n43 * b.n31 + a.n44 * b.n41;

	r.n12 = a.n11 * b.n12 + a.n12 * b.n22 + a.n13 * b.n32 + a.n14 * b.n42;
	r.n22 = a.n21 * b.n12 + a.n22 * b.n22 + a.n23 * b.n32 + a.n24 * b.n42;
	r.n32 = a.n31 * b.n12 + a.n32 * b.n22 + a.n33 * b.n32 + a.n34 * b.n42;
	r.n42 = a.n41 * b.n12 + a.n42 * b.n22 + a.n43 * b.n32 + a.n44 * b.n42;

	r.n13 = a.n11 * b.n13 + a.n12 * b.n23 + a.n13 * b.n33 + a.n14 * b.n43;
	r.n23 = a.n21 * b.n13 + a.n22 * b.n23 + a.n23 * b.n33 + a.n24 * b.n43;
	r.n33 = a.n31 * b.n13 + a.n32 * b.n23 + a.n33 * b.n33 + a.n34 * b.n43;
	r.n43 = a.n41 * b.n13 + a.n42 * b.n23 + a.n43 * b.n33 + a.n44 * b.n43;

	r.n14 = a.n11 * b.n14 + a.n12 * b.n24 + a.n13 * b.n34 + a.n14 * b.n44;
	r.n24 = a.n21 * b.n14 + a.n22 * b.n24 + a.n23 * b.n34 + a.n24 * b.n44;
	r.n34 = a.n31 * b.n14 + a.n32 * b.n24 + a.n33 * b.n34 + a.n34 * b.n44;
	r.n44 = a.n41 * b.n14 + a.n42 * b.n24 + a.n43 * b.n34 + a.n44 * b.n44;

	return r;
}

namespace Math {

	constexpr void transpose(Matrix4* in, Matrix4* out) {
		Matrix4& m = *in;
		Matrix4& r = *out;

		r.n11 = m.n11; r.n12 = m.n21; r.n13 = m.n31; r.n14 = m.n41;
		r.n21 = m.n12; r.n22 = m.n22; r.n23 = m.n32; r.n24 = m.n42;
		r.n31 = m.n13; r.n32 = m.n23; r.n33 = m.n33; r.n34 = m.n43;
		r.n41 = m.n14; r.n42 = m.n24; r.n43 = m.n34; r.n44 = m.n44;
	}

	constexpr void inverse_affine(Matrix4* in, Matrix4* out) {
		Matrix4& m = *in;
		Matrix4& r = *out;

		// transpose rotation
		r.n11 = m.n11; r.n12 = m.n21; r.n13 = m.n31;
		r.n21 = m.n12; r.n22 = m.n22; r.n23 = m.n32;
		r.n31 = m.n13; r.n32 = m.n23; r.n33 = m.n33;

		// compute -R^T * T
		r.n14 = -(r.n11 * m.n14 + r.n12 * m.n24 + r.n13 * m.n34);
		r.n24 = -(r.n21 * m.n14 + r.n22 * m.n24 + r.n23 * m.n34);
		r.n34 = -(r.n31 * m.n14 + r.n32 * m.n24 + r.n33 * m.n34);

		// last row
		r.n41 = 0.0f;
		r.n42 = 0.0f;
		r.n43 = 0.0f;
		r.n44 = 1.0f;
	}

} // namespace Math

#endif // CORE_MATH_MATRIX4_HPP
