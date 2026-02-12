#ifndef RENDER_HPP
#define RENDER_HPP

#include <core/core.hpp>

using namespace core::core;

struct RendererMemory {
	Memory permanent;
	Memory transient;
};

struct Canvas {
	void* memory;
	u32 w;
	u32 h;
	usize pitch;
	Vector2i origin;
};

void init_renderer(RendererMemory* memory, wchar* path_to_assets);

void render(Canvas* canvas);

#endif // RENDER_HPP
