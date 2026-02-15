#include <core/memory/arena.hpp>

namespace Memory {

	void init(Arena* arena, usize size, void* base) {
		assert(size > 0 && base != nullptr);
		arena->base = (byte*)base;
		arena->size = size;
		arena->used = 0;
		arena->restore_point_count = 0;
	}

	void* push(Arena* arena, usize size, usize alignment) {
		assert(size > 0 && alignment > 0);
		usize total_size = size;
		usize alignment_offset = get_alignment_offset((uptr)arena->base + arena->used, alignment);
		total_size += alignment_offset;

		assert(arena->used + total_size <= arena->size);
		void* result = arena->base + arena->used + alignment_offset;
		arena->used += total_size;

		assert(total_size >= size);
		return result;
	}

	Restore_Point begin_temporary(Arena* arena) {
		Restore_Point restore_point;
		restore_point.arena = arena;
		restore_point.used = arena->used;
		++arena->restore_point_count;
		return restore_point;
	}

	void end_temporary(Restore_Point restore_point) {
		Arena* arena = restore_point.arena;
		assert(arena->used >= restore_point.used);
		arena->used = restore_point.used;
		assert(arena->restore_point_count > 0);
		--arena->restore_point_count;
	}

	usize remaining_size(Arena* arena, usize Alignment) {
		return arena->size - (arena->used + get_alignment_offset((usize)arena->base + arena->used, Alignment));
	}

} // namespace Memory

