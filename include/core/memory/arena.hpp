#ifndef CORE_MEMORY_ARENA_HPP
#define CORE_MEMORY_ARENA_HPP

#include "./types.hpp"

namespace core::memory {
	struct Arena {
		byte* base;
		s64 size;
		s64 used;

		s64 restore_point_count;
	};
} // namespace core::memory

namespace core::memory::arena {

	struct RestorePoint {
		Arena* arena;
		s64 used;
	};

	void init(Arena* arena, s64 size, void* base);

	void* push(Arena* arena, s64 size, s64 alignment);

	RestorePoint begin_temporary_memory(Arena* arena);
	void end_temporary_memory(RestorePoint restore_point);

	// TODO: extend arena
	//inline void ZeroSize(memory_index Size, void* Ptr)
	//inline void SubArena( memory_arena* Result, memory_arena * Arena, memory_index Size, memory_index Alignment = 16)
	//inline memory_index GetArenaSizeRemaining(memory_arena* Arena, memory_index Alignment = 4)

} // namespace core::memory::arena

#endif // CORE_MEMORY_ARENA_HPP
