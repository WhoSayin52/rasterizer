#ifndef CORE_MEMORY_ARENA_HPP
#define CORE_MEMORY_ARENA_HPP

#include "./types.hpp"

namespace core::memory {
	struct Arena {
		byte* base;
		s64 size;
		s64 used;
	};
} // namespace core::memory

namespace core::memory::arena {

	void init(Arena* arena, s64 size, void* base);

	void* push(Arena* arena, s64 size, s64 alignment);

} // namespace core::memory::arena

#endif // CORE_MEMORY_ARENA_HPP
