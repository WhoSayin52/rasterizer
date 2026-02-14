#ifndef CORE_MATH_VECTOR3I_HPP
#define CORE_MATH_VECTOR3I_HPP

#include "./../types.hpp"

#pragma warning(push)
#pragma warning(disable: 4201)
struct Vector3i {
	union {
		struct { s32 x, y, z; };
		struct { s32 w, h, l; };
		struct { s32 r, g, b; };
		s32 data[3];
	};
};
#pragma warning(pop)

// operator overloading
constexpr Vector3i operator+(Vector3i v) { return v; }
constexpr Vector3i operator-(Vector3i v) { return { -v.x, -v.y, -v.z }; }

constexpr Vector3i& operator+=(Vector3i& v1, Vector3i v2) { (v1.x += v2.x); (v1.y += v2.y); (v1.z += v2.z); return v1; }
constexpr Vector3i& operator-=(Vector3i& v1, Vector3i v2) { (v1.x -= v2.x); (v1.y -= v2.y); (v1.z -= v2.z); return v1; }
constexpr Vector3i& operator*=(Vector3i& v1, Vector3i v2) { (v1.x *= v2.x); (v1.y *= v2.y); (v1.z *= v2.z); return v1; }

constexpr Vector3i operator+(Vector3i v1, Vector3i v2) { return { (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z) }; }
constexpr Vector3i operator-(Vector3i v1, Vector3i v2) { return { (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z) }; }
constexpr Vector3i operator*(Vector3i v1, Vector3i v2) { return { (v1.x * v2.x), (v1.y * v2.y), (v1.z * v2.z) }; }

constexpr Vector3i operator*(Vector3i v, s32 scaler) { return { (v.x * scaler), (v.y * scaler), (v.z * scaler) }; }
constexpr Vector3i operator*(s32 scaler, Vector3i v) { return v * scaler; }

constexpr bool operator==(Vector3i v1, Vector3i v2) { return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z); }
constexpr bool operator!=(Vector3i v1, Vector3i v2) { return (v1.x != v2.x) || (v1.y != v2.y) && (v1.z != v2.z); }

#endif // CORE_MATH_VECTOR3I_HPP
