#ifndef CORE_MATH_VECTOR4_HPP
#define CORE_MATH_VECTOR4_HPP

#include "./../types.hpp"

// struct
#pragma warning(push)
#pragma warning(disable: 4201)
struct Vector4 {
	union {
		struct { f32 x, y, z, w; };
		struct { f32 w, h, l, t; };
		struct { f32 r, g, b, a; };
		f32 data[4];
	};
};
#pragma warning(pop)

// operator overloading
constexpr Vector4 operator+(Vector4 v) { return v; }
constexpr Vector4 operator-(Vector4 v) { return { -v.x, -v.y, -v.z }; }

constexpr Vector4& operator+=(Vector4& v1, Vector4 v2) { (v1.x += v2.x); (v1.y += v2.y); (v1.z += v2.z); (v1.w += v2.w); return v1; }
constexpr Vector4& operator-=(Vector4& v1, Vector4 v2) { (v1.x -= v2.x); (v1.y -= v2.y); (v1.z -= v2.z); (v1.w -= v2.w); return v1; }
constexpr Vector4& operator*=(Vector4& v1, Vector4 v2) { (v1.x *= v2.x); (v1.y *= v2.y); (v1.z *= v2.z); (v1.w *= v2.w); return v1; }

constexpr Vector4 operator+(Vector4 v1, Vector4 v2) { return { (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z), (v1.w + v2.w) }; }
constexpr Vector4 operator-(Vector4 v1, Vector4 v2) { return { (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z), (v1.w - v2.w) }; }
constexpr Vector4 operator*(Vector4 v1, Vector4 v2) { return { (v1.x * v2.x), (v1.y * v2.y), (v1.z * v2.z), (v1.w * v2.w) }; }

constexpr Vector4 operator*(Vector4 v, f32 scaler) { return { (v.x * scaler), (v.y * scaler), (v.z * scaler), (v.w * scaler) }; }
constexpr Vector4 operator*(f32 scaler, Vector4 v) { return v * scaler; }

constexpr bool operator==(Vector4 v1, Vector4 v2) { return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w); }
constexpr bool operator!=(Vector4 v1, Vector4 v2) { return (v1.x != v2.x) || (v1.y != v2.y) && (v1.z != v2.z) && (v1.w != v2.w); }

namespace Math {
	// functions
	constexpr f32 dot(Vector4 v1, Vector4 v2) {
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
	}

	f32 length(Vector4 v);

	Vector4 normalize(Vector4 v);

	constexpr Vector4 reflect(Vector4 vector, Vector4 normal) {
		return (normal * (2.0f * dot(vector, normal))) - vector;
	}

} // namespace Math

#endif // CORE_MATH_VECTOR4_HPP
