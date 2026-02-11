#include <core/memory/arena.hpp>

namespace core::memory::arena {

	void init(Arena* arena, s64 size, void* base) {
		ASSERT(base != nullptr && size > 0);
		arena->base = (byte*)base;
		arena->size = size;
		arena->used = 0;
		arena->restore_point_count = 0;
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

	RestorePoint begin_temporary_memory(Arena* arena) {
		RestorePoint restore_point;
		restore_point.arena = arena;
		restore_point.used = arena->used;
		++arena->restore_point_count;
		return restore_point;
	}

	void end_temporary_memory(RestorePoint restore_point) {
		Arena* arena = restore_point.arena;
		ASSERT(arena->used >= restore_point.used);
		arena->used = restore_point.used;
		ASSERT(arena->restore_point_count > 0);
		--arena->restore_point_count;
	}

} // namespace core::memory::arena

