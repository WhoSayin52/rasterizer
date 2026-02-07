#ifndef MATH_HPP
#define MATH_HPP

#include "./vector/vector.hpp"

#include <core/core.hpp>

namespace math {

	// constants
	constexpr f64 pi = 3.141592653589793;

	// utility functions
	template <typename T>
	T minimum(T a, T b) {
		return (a < b) ? a : b;
	}

	template <typename T>
	T maximum(T a, T b) {
		return (a > b) ? a : b;
	}

	template <typename T>
	T clamp(T value, T min, T max) {
		return minimum(maximum(value, min), max);
	}

} // namespace math

#endif // MATH_HPP
