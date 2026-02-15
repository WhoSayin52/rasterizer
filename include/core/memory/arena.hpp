#ifndef CORE_MEMORY_ARENA_HPP
#define CORE_MEMORY_ARENA_HPP

#include "./types.hpp"

namespace Memory {

	struct Arena {
		byte* base;
		usize size;
		usize used;

		i32 restore_point_count;
	};

	struct Restore_Point {
		Arena* arena;
		usize used;
	};

	void init(Arena* arena, usize size, void* base);

	void* push(Arena* arena, usize size, usize alignment);

	Restore_Point begin_temporary(Arena* arena);
	void end_temporary(Restore_Point restore_point);

	usize remaining_size(Arena* arena, usize Alignment);

	// TODO: extend arena
	//inline void ZeroSize(memory_index Size, void* Ptr)
	//inline void SubArena( memory_arena* Result, memory_arena * Arena, memory_index Size, memory_index Alignment = 16)

} // namespace Memory

#endif // CORE_MEMORY_ARENA_HPP
