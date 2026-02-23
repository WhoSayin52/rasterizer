#ifndef CORE_MATH_MATRIX2_HPP
#define CORE_MATH_MATRIX2_HPP

#include "./../types.hpp"
#include "./../vector/vector2.hpp"

#pragma warning(push)
#pragma warning(disable: 4201)
// IMPORTANT Column-major
struct Matrix2 {
	union {
		f32 data[4];
		struct {
			f32 n11, n21; // col 1
			f32 n12, n22; // col 2
		};
		struct {
			Vector2 col1;
			Vector2 col2;
		};
		struct {
			Vector2 x;
			Vector2 y;
		};
		struct {
			Vector2 i;
			Vector2 j;
		};
		struct {
			Vector2 x_axis;
			Vector2 y_axis;
		};
		struct {
			Vector2 right;
			Vector2 up;
		};
	};
};
#pragma warning(pop)

constexpr Vector2 operator*(const Matrix2& A, const Vector2& x) {
	return x.x * A.col1 + x.y * A.col2;
}

constexpr Matrix2 operator*(const Matrix2& a, const Matrix2& b) {
	Matrix2 r;

	r.n11 = a.n11 * b.n11 + a.n12 * b.n21;
	r.n21 = a.n21 * b.n11 + a.n22 * b.n21;

	r.n12 = a.n11 * b.n12 + a.n12 * b.n22;
	r.n22 = a.n21 * b.n12 + a.n22 * b.n22;

	return r;
}

namespace Math {

	constexpr Matrix2 transpose(Matrix2* in, Matrix2* out) {
		Matrix2& m = *in;
		Matrix2& r = *out;

		r.n11 = m.n11;
		r.n12 = m.n21;

		r.n21 = m.n12;
		r.n22 = m.n22;

		return r;
	}

	constexpr f32 determinant(Matrix2* in) {
		Matrix2& m = *in;
		return m.n11 * m.n22 - m.n12 * m.n21;
	}

	constexpr void inverse(Matrix2* in, Matrix2* out) {
		f32 det = determinant(in);
		f32 inv_det = 1.0f / det;

		Matrix2& m = *in;
		Matrix2& r = *out;

		r.n11 = m.n22 * inv_det;
		r.n12 = -m.n12 * inv_det;

		r.n21 = -m.n21 * inv_det;
		r.n22 = m.n11 * inv_det;
	}

} // namespace Math

#endif // CORE_MATH_MATRIX2_HPP
