#include "./render.hpp"

static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, Vector3 color);
static void set_pixel(Canvas* canvas, s64 x, s64 y, Vector3 color);

void render(Canvas* canvas) {

	draw_line(canvas, { 60, 240 }, { -50, -200 }, { 1.0, 1.0, 1.0 });
	draw_line(canvas, { 140, 120 }, { -100, -100 }, { 1.0, 1.0, 1.0 });
	draw_line(canvas, { 0, 200 }, { 0, -200 }, { 1.0, 1.0, 1.0 });
	draw_line(canvas, { 200, 0 }, { -200, 0 }, { 1.0, 1.0, 1.0 });
}

static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, Vector3 color) {
	// Bresenhams Algorithm
	s64 x0 = p0.x;
	s64 x1 = p1.x;
	s64 y0 = p0.y;
	s64 y1 = p1.y;

	bool steep = math::abs(x0 - x1) < math::abs(y0 - y1);
	if (steep) { // if the line is steep, we transpose the image
		math::swap(&x0, &y0);
		math::swap(&x1, &y1);
	}
	if (x0 > x1) { // make it left−to−right
		math::swap(&x0, &x1);
		math::swap(&y0, &y1);
	}

	s64 ierror = 0;
	s64 y = y0;
	for (s64 x = x0; x <= x1; ++x) {
		if (steep) // if transposed, de−transpose
			set_pixel(canvas, y, x, color);
		else
			set_pixel(canvas, x, y, color);
		ierror += 2 * math::abs(y1 - y0);
		y += (y1 > y0 ? 1 : -1) * (ierror > x1 - x0);
		ierror -= 2 * (x1 - x0) * (ierror > x1 - x0);
	}
}

static void set_pixel(Canvas* canvas, s64 x, s64 y, Vector3 color) {
	x += canvas->origin.x;
	y += canvas->origin.y;

	ASSERT(
		(usize)x < canvas->w &&
		(usize)y < canvas->h &&
		canvas->memory != nullptr
	);

	usize bpp = 4; // byte per pixel

	u8 red = (u8)math::clamp(color.r * 255.0, 0.0, 255.0);
	u8 green = (u8)math::clamp(color.g * 255.0, 0.0, 255.0);
	u8 blue = (u8)math::clamp(color.b * 255.0, 0.0, 255.0);

	u32* pixel = (u32*)((byte*)canvas->memory + y * canvas->pitch + x * bpp);

	*pixel = (u32)red << 16 | green << 8 | blue;
}


