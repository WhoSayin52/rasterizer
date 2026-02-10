#ifndef CORE_MEMORY_TYPES_HPP
#define CORE_MEMORY_TYPES_HPP

#include "./../types.hpp"

namespace core::memory {
	// memory
	struct Memory {
		void* base;
		usize size;
	};

} // namespace core::memory

namespace core::memory::memory {

	constexpr usize kilobytes(usize bytes) { return bytes * 1024LL; }
	constexpr usize megabytes(usize bytes) { return kilobytes(bytes) * 1024LL; }
	constexpr usize gigabytes(usize bytes) { return megabytes(bytes) * 1024LL; }
	constexpr usize terabytes(usize bytes) { return gigabytes(bytes) * 1024LL; }

	constexpr usize align4(usize bytes) { return (bytes + 3) & ~3; }

	constexpr usize get_alignment_offset(usize index, usize alignment) {
		ASSERT(alignment % 2 == 0);
		usize alignment_mask = alignment - 1;
		usize offset = 0;
		if (index & alignment_mask) {
			offset = alignment - (index & alignment_mask);
		}
		return offset;
	}

} // namespace core::memory::memory

#endif // CORE_MEMORY_TYPES_HPP
