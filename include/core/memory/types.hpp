#ifndef CORE_MEMORY_TYPES_HPP
#define CORE_MEMORY_TYPES_HPP

#include "./../types.hpp"

namespace core::memory {
	// memory
	struct Memory {
		void* base;
		s64 size;
	};

} // namespace core::memory

namespace core::memory::memory {

	constexpr s64 kilobytes(s64 bytes) { return bytes * 1024LL; }
	constexpr s64 megabytes(s64 bytes) { return kilobytes(bytes) * 1024LL; }
	constexpr s64 gigabytes(s64 bytes) { return megabytes(bytes) * 1024LL; }
	constexpr s64 terabytes(s64 bytes) { return gigabytes(bytes) * 1024LL; }

	constexpr s64 align_up(s64 bytes, s64 alignment) {
		ASSERT(bytes >= 0 && alignment > 0 && alignment % 2 == 0);
		return (bytes + (alignment - 1)) & ~(alignment - 1);
	}
	constexpr s64 align4(s64 bytes) { return align_up(bytes, 4); }

	constexpr s64 get_alignment_offset(s64 index, s64 alignment) {
		ASSERT(index >= 0 && alignment > 0 && alignment % 2 == 0);
		s64 alignment_mask = alignment - 1;
		s64 offset = 0;
		if (index & alignment_mask) {
			offset = alignment - (index & alignment_mask);
		}
		return offset;
	}

} // namespace core::memory::memory

#endif // CORE_MEMORY_TYPES_HPP
