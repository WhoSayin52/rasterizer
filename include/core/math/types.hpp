#ifndef CORE_MATH_TYPES_HPP
#define CORE_MATH_TYPES_HPP

#include "./../types.hpp"

namespace core::math::math {

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

	template <typename T>
	T abs(T value) {
		return value >= 0 ? value : -value;
	}

	template <typename T>
	void swap(T* a, T* b) {
		T temp = *a;
		*a = *b;
		*b = temp;
	}

} // namespace core::math::math

#endif // CORE_MATH_TYPES_HPP
