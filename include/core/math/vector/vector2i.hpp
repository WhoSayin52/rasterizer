#ifndef CORE_MATH_VECTOR2I_HPP
#define CORE_MATH_VECTOR2I_HPP

#include "./../types.hpp"

namespace core::math {

	// struct
#pragma warning(push)
#pragma warning(disable: 4201)
	struct Vector2i {
		union {
			struct { s64 x, y; };
			struct { s64 w, h; };
			struct { s64 min, max; };
			s64 data[2];
		};
	};
#pragma warning(pop)

	// operator overloading
	constexpr Vector2i operator+(Vector2i v) { return v; }
	constexpr Vector2i operator-(Vector2i v) { return { -v.x, -v.y }; }

	constexpr Vector2i& operator+=(Vector2i& v1, Vector2i v2) { (v1.x += v2.x); (v1.y += v2.y); return v1; }
	constexpr Vector2i& operator-=(Vector2i& v1, Vector2i v2) { (v1.x -= v2.x); (v1.y -= v2.y); return v1; }
	constexpr Vector2i& operator*=(Vector2i& v1, Vector2i v2) { (v1.x *= v2.x); (v1.y *= v2.y); return v1; }

	constexpr Vector2i operator+(Vector2i v1, Vector2i v2) { return { (v1.x + v2.x), (v1.y + v2.y) }; }
	constexpr Vector2i operator-(Vector2i v1, Vector2i v2) { return { (v1.x - v2.x), (v1.y - v2.y) }; }
	constexpr Vector2i operator*(Vector2i v1, Vector2i v2) { return { (v1.x * v2.x), (v1.y * v2.y) }; }

	constexpr Vector2i operator*(Vector2i v, s64 scaler) { return { (v.x * scaler), (v.y * scaler) }; }
	constexpr Vector2i operator*(s64 scaler, Vector2i v) { return v * scaler; }

	constexpr bool operator==(Vector2i v1, Vector2i v2) { return (v1.x == v2.x) && (v1.y == v2.y); }
	constexpr bool operator!=(Vector2i v1, Vector2i v2) { return (v1.x != v2.x) || (v1.y != v2.y); }
} // namespace core::math

#endif // CORE_MATH_VECTOR2I_HPP
