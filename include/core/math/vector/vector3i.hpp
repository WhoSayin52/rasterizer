#ifndef CORE_MATH_VECTOR3I_HPP
#define CORE_MATH_VECTOR3I_HPP

#include "./../types.hpp"

namespace core::math {

#pragma warning(push)
#pragma warning(disable: 4201)
	struct Vector3i {
		union {
			struct { s64 x, y, z; };
			struct { s64 w, h, l; };
			struct { s64 r, g, b; };
			s64 data[3];
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

	constexpr Vector3i operator*(Vector3i v, s64 scaler) { return { (v.x * scaler), (v.y * scaler), (v.z * scaler) }; }
	constexpr Vector3i operator*(s64 scaler, Vector3i v) { return v * scaler; }

	constexpr bool operator==(Vector3i v1, Vector3i v2) { return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z); }
	constexpr bool operator!=(Vector3i v1, Vector3i v2) { return (v1.x != v2.x) || (v1.y != v2.y) && (v1.z != v2.z); }

} // namespace core::math{

#endif // CORE_MATH_VECTOR3I_HPP
