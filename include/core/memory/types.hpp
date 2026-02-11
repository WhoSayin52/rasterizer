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

	constexpr usize kilobytes(usize bytes) { return bytes * 1024ULL; }
	constexpr usize megabytes(usize bytes) { return kilobytes(bytes) * 1024ULL; }
	constexpr usize gigabytes(usize bytes) { return megabytes(bytes) * 1024ULL; }
	constexpr usize terabytes(usize bytes) { return gigabytes(bytes) * 1024ULL; }

	constexpr bool is_power_2(usize value) { return (value & (value - 1)) == 0; }

	constexpr usize align_up(usize bytes, usize alignment) {
		ASSERT(alignment != 0 && is_power_2(alignment));
		return (bytes + (alignment - 1)) & ~(alignment - 1);
	}
	constexpr usize align4(usize bytes) { return align_up(bytes, 4); }

	constexpr usize get_alignment_offset(uptr address, usize alignment) {
		ASSERT(alignment != 0 && is_power_2(alignment));
		usize alignment_mask = alignment - 1;
		usize offset = 0;
		if (address & alignment_mask) {
			offset = alignment - (address & alignment_mask);
			ASSERT(offset <= alignment);
		}
		return offset;
	}

} // namespace core::memory::memory

#endif // CORE_MEMORY_TYPES_HPP
