#include "./render.hpp"

#include "./../asset_manager/asset_manager.hpp"

// static global vars
static Model global_diablo;

// internal functions
static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, Vector3 color);
static void set_pixel(Canvas* canvas, i32 x, i32 y, Vector3 color);

void init_renderer(Renderer_Memory* memory, wchar* path_to_assets) {
	set_asset_manager_path(path_to_assets);

	load_model(memory, &global_diablo, L"diablo3_pose.obj");
}

void render(Canvas* canvas) {

	draw_line(canvas, { 60, 240 }, { -50, -200 }, { 1.0, 1.0, 1.0 });
	draw_line(canvas, { 140, 120 }, { -100, -100 }, { 1.0, 1.0, 1.0 });
	draw_line(canvas, { 0, 200 }, { 0, -200 }, { 1.0, 1.0, 1.0 });
	draw_line(canvas, { 200, 0 }, { -200, 0 }, { 1.0, 1.0, 1.0 });
}

static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, Vector3 color) {
	// Bresenhams Algorithm
	i32 x0 = p0.x;
	i32 x1 = p1.x;
	i32 y0 = p0.y;
	i32 y1 = p1.y;

	bool steep = Math::abs(x0 - x1) < Math::abs(y0 - y1);
	if (steep) { // if the line is steep, we transpose the image
		Math::swap(&x0, &y0);
		Math::swap(&x1, &y1);
	}
	if (x0 > x1) { // make it left−to−right
		Math::swap(&x0, &x1);
		Math::swap(&y0, &y1);
	}

	i32 ierror = 0;
	i32 y = y0;
	for (i32 x = x0; x <= x1; ++x) {
		if (steep) // if transposed, de−transpose
			set_pixel(canvas, y, x, color);
		else
			set_pixel(canvas, x, y, color);
		ierror += 2 * Math::abs(y1 - y0);
		y += (y1 > y0 ? 1 : -1) * (ierror > x1 - x0);
		ierror -= 2 * (x1 - x0) * (ierror > x1 - x0);
	}
}

static void set_pixel(Canvas* canvas, i32 x, i32 y, Vector3 color) {
	u32 cx = x + canvas->origin.x;
	u32 cy = y + canvas->origin.y;

	assert(
		cx < canvas->w &&
		cy < canvas->h &&
		canvas->memory != nullptr
	);

	u32 bpp = 4; // byte per pixel

	u8 red = (u8)Math::clamp(color.r * 255.0f, 0.0f, 255.0f);
	u8 green = (u8)Math::clamp(color.g * 255.0f, 0.0f, 255.0f);
	u8 blue = (u8)Math::clamp(color.b * 255.0f, 0.0f, 255.0f);

	u32* pixel = (u32*)((byte*)canvas->memory + cy * canvas->pitch + cx * bpp);

	*pixel = (u32)red << 16 | green << 8 | blue;
}


