#ifndef RENDER_HPP
#define RENDER_HPP

#include <core/core.hpp>

struct RendererMemory {
	void* permanent_memory;
	usize permanent_memory_size;

	void* transient_memory;
	usize transient_memory_size;
};

struct Canvas {
	void* memory;
	u32 w;
	u32 h;
	u32 pitch;
	Vector2i origin;
};

void init_renderer(RendererMemory* memory, wchar* path_to_assets);

void render(Canvas* canvas);

#endif // RENDER_HPP
