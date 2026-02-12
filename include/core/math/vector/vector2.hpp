#ifndef CORE_MATH_VECTOR2_HPP
#define CORE_MATH_VECTOR2_HPP

#include "./../types.hpp"

namespace core::math {

	// struct
#pragma warning(push)
#pragma warning(disable: 4201)
	struct Vector2 {
		union {
			struct { f32 x, y; };
			struct { f32 w, h; };
			struct { f32 min, max; };
			f32 data[2];
		};
	};
#pragma warning(pop)

	// operator overloading
	constexpr Vector2 operator+(Vector2 v) { return v; }
	constexpr Vector2 operator-(Vector2 v) { return { -v.x, -v.y }; }

	constexpr Vector2& operator+=(Vector2& v1, Vector2 v2) { (v1.x += v2.x); (v1.y += v2.y); return v1; }
	constexpr Vector2& operator-=(Vector2& v1, Vector2 v2) { (v1.x -= v2.x); (v1.y -= v2.y); return v1; }
	constexpr Vector2& operator*=(Vector2& v1, Vector2 v2) { (v1.x *= v2.x); (v1.y *= v2.y); return v1; }

	constexpr Vector2 operator+(Vector2 v1, Vector2 v2) { return { (v1.x + v2.x), (v1.y + v2.y) }; }
	constexpr Vector2 operator-(Vector2 v1, Vector2 v2) { return { (v1.x - v2.x), (v1.y - v2.y) }; }
	constexpr Vector2 operator*(Vector2 v1, Vector2 v2) { return { (v1.x * v2.x), (v1.y * v2.y) }; }

	constexpr Vector2 operator*(Vector2 v, f32 scaler) { return { (v.x * scaler), (v.y * scaler) }; }
	constexpr Vector2 operator*(f32 scaler, Vector2 v) { return v * scaler; }

	constexpr bool operator==(Vector2 v1, Vector2 v2) { return (v1.x == v2.x) && (v1.y == v2.y); }
	constexpr bool operator!=(Vector2 v1, Vector2 v2) { return (v1.x != v2.x) || (v1.y != v2.y); }
	
} // namespace core::math

namespace core::math::vector {
	// functions
	constexpr f32 dot(Vector2 v1, Vector2 v2) {
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	f32 length(Vector2 v);

	Vector2 normalize(Vector2 v);

	constexpr Vector2 reflect(Vector2 vector, Vector2 normal) {
		return (normal * (2.0f * dot(vector, normal))) - vector;
	}
} // namespace core::math::vector{

#endif // CORE_MATH_VECTOR2_HPP
