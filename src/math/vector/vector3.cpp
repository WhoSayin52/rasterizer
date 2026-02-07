#include <math/vector/vector3.hpp>

#include <cmath>

// functions
f64 length(Vector3 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

Vector3 normalize(Vector3 v) {
	f64 magnitude = length(v);
	ASSERT(magnitude != 0.0);
	return { (v.x / magnitude), (v.y / magnitude), (v.z / magnitude) };
}
