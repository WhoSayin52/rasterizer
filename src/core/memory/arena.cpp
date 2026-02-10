#include <core/memory/arena.hpp>

namespace core::memory::arena {

	void init(Arena* arena, usize size, void* base) {
		ASSERT(base != nullptr);
		arena->base = (byte*)base;
		arena->size = size;
		arena->used = 0;
	}

	void* push(Arena* arena, usize size, usize alignment) {
		usize total_size = size;
		usize alignment_offset = memory::get_alignment_offset((usize)arena->base + arena->used, alignment);
		total_size += alignment_offset;

		ASSERT(arena->used + total_size <= arena->size);
		void* result = arena->base + arena->used + alignment_offset;
		arena->used += total_size;

		ASSERT(total_size >= size);
		return result;
	}

} // namespace core::memory::arena

