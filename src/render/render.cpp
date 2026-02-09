#include "./render.hpp"

static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, Vector3 color);
static void set_pixel(Canvas* canvas, usize x, usize y, Vector3 color);

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

	s64 dx = math::abs(x1 - x0);
	s64 sx = x0 < x1 ? 1 : -1;
	s64 dy = -math::abs(y1 - y0);
	s64 sy = y0 < y1 ? 1 : -1;
	s64 error = dx + dy;

	while (true) {
		set_pixel(canvas, x0, y0, color);
		s64 e2 = 2 * error;
		if (e2 >= dy) {
			if (x0 == x1) {
				break;
			}
			error = error + dy;
			x0 = x0 + sx;
		}
		if (e2 <= dx) {
			if (y0 == y1) {
				break;
			}
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

static void set_pixel(Canvas* canvas, usize x, usize y, Vector3 color) {
	x += canvas->origin.x;
	y += canvas->origin.y;

	ASSERT(
		x < canvas->w &&
		y < canvas->h &&
		canvas->memory != nullptr
	);

	usize bpp = 4; // byte per pixel

	u8 red = (u8)math::clamp(color.r * 255.0, 0.0, 255.0);
	u8 green = (u8)math::clamp(color.g * 255.0, 0.0, 255.0);
	u8 blue = (u8)math::clamp(color.b * 255.0, 0.0, 255.0);

	u32* pixel = (u32*)((byte*)canvas->memory + y * canvas->pitch + x * bpp);

	*pixel = (u32)red << 16 | green << 8 | blue;
}


