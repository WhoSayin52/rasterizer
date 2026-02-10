#ifndef CORE_MEMORY_ARENA_HPP
#define CORE_MEMORY_ARENA_HPP

#include "./types.hpp"

namespace core::memory {
	struct Arena {
		byte* base;
		usize size;
		usize used;
	};
} // namespace core::memory

namespace core::memory::arena {

	void init(Arena* arena, usize size, void* base);

	void* push(Arena* arena, usize size, usize alignment);

} // namespace core::memory::arena

#endif // CORE_MEMORY_ARENA_HPP
