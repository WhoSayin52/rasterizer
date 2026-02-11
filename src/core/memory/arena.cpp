#include <core/memory/arena.hpp>

namespace core::memory::arena {

	void init(Arena* arena, s64 size, void* base) {
		ASSERT(base != nullptr && size > 0);
		arena->base = (byte*)base;
		arena->size = size;
		arena->used = 0;
	}

	void* push(Arena* arena, s64 size, s64 alignment) {
		ASSERT(size > 0 && alignment > 0)
			s64 total_size = size;
		s64 alignment_offset = memory::get_alignment_offset((s64)arena->base + arena->used, alignment);
		total_size += alignment_offset;

		ASSERT(arena->used + total_size <= arena->size);
		void* result = arena->base + arena->used + alignment_offset;
		arena->used += total_size;

		ASSERT(total_size >= size);
		return result;
	}

} // namespace core::memory::arena

