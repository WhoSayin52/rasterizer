#include <core/math/vector/vector3.hpp>

#include <cmath>

namespace Math {

	// functions
	f32 length(Vector3 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

	Vector3 normalize(Vector3 v) {
		f32 magnitude = length(v);
		assert(magnitude != 0.0f);
		return { (v.x / magnitude), (v.y / magnitude), (v.z / magnitude) };
	}

} // namespace Math 
