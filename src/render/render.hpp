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
	usize w;
	usize h;
	usize pitch;
	Vector2i origin;
};

void init_renderer(RendererMemory* memory, char* assets_path);
void render(Canvas* canvas);

#endif // RENDER_HPP
