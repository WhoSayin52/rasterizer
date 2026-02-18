#ifndef RENDER_HPP
#define RENDER_HPP

#include <core/core.hpp>

struct Renderer_Memory {
	Memory::Arena permanent;
	Memory::Arena transient;
};

struct Canvas {
	void* framebuffer;
	u32 w;
	u32 h;
	u32 pitch;
	Vector2i origin;
};

void init_renderer(Renderer_Memory* memory, wchar* path_to_assets);

void render(Memory::Arena* arena, Canvas* canvas, i32 model);

#endif // RENDER_HPP
