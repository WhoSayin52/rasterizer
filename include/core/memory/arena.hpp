#ifndef CORE_MEMORY_ARENA_HPP
#define CORE_MEMORY_ARENA_HPP

#include "./types.hpp"

namespace Memory {

	struct Arena {
		byte* base;
		usize size;
		usize used;

		i32 snapshot_count;
	};

	struct Arena_Snapshot {
		Arena* arena;
		usize used;
	};

	void arena_init(Arena* arena, usize size, void* base);

	void* arena_push(Arena* arena, usize size, usize alignment);

	Arena_Snapshot arena_create_snapshot(Arena* arena);
	void arena_restore(Arena_Snapshot arena_snapshot);

	usize arena_remaining_size(Arena* arena, usize Alignment);

} // namespace Memory

#endif // CORE_MEMORY_ARENA_HPP
