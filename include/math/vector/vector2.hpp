#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <core/core.hpp>

// struct
#pragma warning(push)
#pragma warning(disable: 4201)
struct Vector2 {
	union {
		struct { f64 x, y; };
		struct { f64 w, h; };
		struct { f64 min, max; };
		f64 data[2];
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

constexpr Vector2 operator*(Vector2 v, f64 scaler) { return { (v.x * scaler), (v.y * scaler) }; }
constexpr Vector2 operator*(f64 scaler, Vector2 v) { return v * scaler; }

constexpr bool operator==(Vector2 v1, Vector2 v2) { return (v1.x == v2.x) && (v1.y == v2.y); }
constexpr bool operator!=(Vector2 v1, Vector2 v2) { return (v1.x != v2.x) || (v1.y != v2.y); }

// functions
constexpr f64 dot(Vector2 v1, Vector2 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

f64 length(Vector2 v);

Vector2 normalize(Vector2 v);

constexpr Vector2 reflect(Vector2 vector, Vector2 normal) {
	return (normal * (2.0f * dot(vector, normal))) - vector;
}

#endif // VECTOR2_HPP
