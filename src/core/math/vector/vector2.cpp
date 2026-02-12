#include <core/math/vector/vector2.hpp>

#include <cmath>

namespace core::math::vector {

	// functions
	f32 length(Vector2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }

	Vector2 normalize(Vector2 v) {
		f32 magnitude = length(v);
		ASSERT(magnitude != 0.0f);
		return { (v.x / magnitude), (v.y / magnitude) };
	}

} // namespace core::math::vector
