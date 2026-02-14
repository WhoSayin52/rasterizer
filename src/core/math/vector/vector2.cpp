#include <core/math/vector/vector2.hpp>

#include <cmath>

namespace Math {

	// functions
	f32 length(Vector2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }

	Vector2 normalize(Vector2 v) {
		f32 magnitude = length(v);
		assert(magnitude != 0.0f);
		return { (v.x / magnitude), (v.y / magnitude) };
	}

} // namespace Math
