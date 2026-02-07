#ifndef RENDER_HPP
#define RENDER_HPP

#include <core/core.hpp>
#include <math/math.hpp>

struct Canvas {
	void* memory;
	usize w;
	usize h;
	usize pitch;
	Vector2i origin;
};

void render(Canvas* canvas);

#endif // RENDER_HPP
