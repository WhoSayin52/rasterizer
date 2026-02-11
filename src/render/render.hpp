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
	s64 w;
	s64 h;
	s64 pitch;
	Vector2i origin;
};

void render(Canvas* canvas);

#endif // RENDER_HPP
