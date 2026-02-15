#include <core/memory/arena.hpp>

namespace Memory {

	void arena_init(Arena* arena, usize size, void* base) {
		assert(size > 0 && base != nullptr);
		arena->base = (byte*)base;
		arena->size = size;
		arena->used = 0;
		arena->snapshot_count = 0;
	}

	void* arena_push(Arena* arena, usize size, usize alignment) {
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

	Arena_Snapshot arena_create_snapshot(Arena* arena) {
		Arena_Snapshot arena_snapshot;
		arena_snapshot.arena = arena;
		arena_snapshot.used = arena->used;
		++arena->snapshot_count;
		return arena_snapshot;
	}

	void arena_restore(Arena_Snapshot arena_snapshot) {
		Arena* arena = arena_snapshot.arena;
		assert(arena->used >= arena_snapshot.used);
		arena->used = arena_snapshot.used;
		assert(arena->snapshot_count > 0);
		--arena->snapshot_count;
	}

	usize arena_remaining_size(Arena* arena, usize Alignment) {
		return arena->size - (arena->used + get_alignment_offset((usize)arena->base + arena->used, Alignment));
	}

} // namespace Memory

