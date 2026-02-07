#include <math/vector/vector2.hpp>

#include <cmath>

// functions
f64 length(Vector2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }

Vector2 normalize(Vector2 v) {
	f64 magnitude = length(v);
	ASSERT(magnitude != 0.0);
	return { (v.x / magnitude), (v.y / magnitude) };
}

