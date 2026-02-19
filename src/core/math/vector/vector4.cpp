#include <core/math/vector/vector4.hpp>

#include <cmath>

namespace Math {

	// functions
	f32 length(Vector4 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w)); }

	Vector4 normalize(Vector4 v) {
		f32 magnitude = length(v);
		assert(magnitude != 0.0f);
		return { (v.x / magnitude), (v.y / magnitude), (v.z / magnitude), (v.w / magnitude) };
	}

} // namespace Math 
