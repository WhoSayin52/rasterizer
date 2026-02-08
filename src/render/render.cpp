#include "./render.hpp"

static void set_pixel(Canvas* canvas, usize x, usize y, Vector3 rgb);

void render(Canvas* canvas) {

	for (usize y = 0; y < canvas->h; ++y) {
		for (usize x = 0; x < canvas->w; ++x) {
			set_pixel(canvas, x, y, Vector3{1.0, 0.0, 0.0});
		}
	}
}

static void set_pixel(Canvas* canvas, usize x, usize y, Vector3 color) {
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

	*pixel = (u32)red << 16 | green << 16 | blue;
}
