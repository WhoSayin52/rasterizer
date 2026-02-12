#ifndef CORE_MATH_VECTOR3_HPP
#define CORE_MATH_VECTOR3_HPP

#include "./../types.hpp"

namespace core::math {

	// struct
#pragma warning(push)
#pragma warning(disable: 4201)
	struct Vector3 {
		union {
			struct { f32 x, y, z; };
			struct { f32 w, h, l; };
			struct { f32 r, g, b; };
			f32 data[3];
		};
	};
#pragma warning(pop)

	// operator overloading
	constexpr Vector3 operator+(Vector3 v) { return v; }
	constexpr Vector3 operator-(Vector3 v) { return { -v.x, -v.y, -v.z }; }

	constexpr Vector3& operator+=(Vector3& v1, Vector3 v2) { (v1.x += v2.x); (v1.y += v2.y); (v1.z += v2.z); return v1; }
	constexpr Vector3& operator-=(Vector3& v1, Vector3 v2) { (v1.x -= v2.x); (v1.y -= v2.y); (v1.z -= v2.z); return v1; }
	constexpr Vector3& operator*=(Vector3& v1, Vector3 v2) { (v1.x *= v2.x); (v1.y *= v2.y); (v1.z *= v2.z); return v1; }

	constexpr Vector3 operator+(Vector3 v1, Vector3 v2) { return { (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z) }; }
	constexpr Vector3 operator-(Vector3 v1, Vector3 v2) { return { (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z) }; }
	constexpr Vector3 operator*(Vector3 v1, Vector3 v2) { return { (v1.x * v2.x), (v1.y * v2.y), (v1.z * v2.z) }; }

	constexpr Vector3 operator*(Vector3 v, f32 scaler) { return { (v.x * scaler), (v.y * scaler), (v.z * scaler) }; }
	constexpr Vector3 operator*(f32 scaler, Vector3 v) { return v * scaler; }

	constexpr bool operator==(Vector3 v1, Vector3 v2) { return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z); }
	constexpr bool operator!=(Vector3 v1, Vector3 v2) { return (v1.x != v2.x) || (v1.y != v2.y) && (v1.z != v2.z); }

} // namespace core::math

namespace core::math::vector {
	// functions
	constexpr f32 dot(Vector3 v1, Vector3 v2) {
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}
	constexpr Vector3 cross(Vector3 v1, Vector3 v2) {
		return { (v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x) };
	}

	f32 length(Vector3 v);

	Vector3 normalize(Vector3 v);

	constexpr Vector3 reflect(Vector3 vector, Vector3 normal) {
		return (normal * (2.0f * dot(vector, normal))) - vector;
	}
} // namespace core::math::vector

#endif // CORE_MATH_VECTOR3_HPP
