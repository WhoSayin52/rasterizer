#include "./draw.hpp"

#include "./render.hpp"

#include <cstdlib>

void draw_filled_triangle(Canvas* canvas, Canvas* z_buffer, Vector3 v1, Vector3 v2, Vector3 v3, u32 color) {
	Vector2i p1, p2, p3;

	p1.x = (i32)v1.x;
	p1.y = (i32)v1.y;

	p2.x = (i32)v2.x;
	p2.y = (i32)v2.y;

	p3.x = (i32)v3.x;
	p3.y = (i32)v3.y;

	// calculating the min and max point of the bounding box
	i32 min_x = Math::minimum(p1.x, Math::minimum(p2.x, p3.x));
	i32 min_y = Math::minimum(p1.y, Math::minimum(p2.y, p3.y));
	i32 max_x = Math::maximum(p1.x, Math::maximum(p2.x, p3.x));
	i32 max_y = Math::maximum(p1.y, Math::maximum(p2.y, p3.y));

	f32 total_area2 = signed_triangle_area2(p1, p2, p3);
	if (Math::abs(total_area2) < 2.0f) {
		return;
	}

#pragma omp parallel for
	for (i32 y = min_y; y <= max_y; ++y) {
		for (i32 x = min_x; x <= max_x; ++x) {
			f32 a = signed_triangle_area2(Vector2i{ x, y }, p2, p3) / total_area2;
			f32 b = signed_triangle_area2(Vector2i{ x, y }, p3, p1) / total_area2;
			f32 c = signed_triangle_area2(Vector2i{ x, y }, p1, p2) / total_area2;

			bool is_valid_pixel = a >= 0 && b >= 0 && c >= 0;
			if (is_valid_pixel) {
				f32 z = a * v1.z + b * v2.z + c * v3.z;
				if (z < get_z_buffer_at(z_buffer, x, y)) {
					set_z_buffer_at(z_buffer, x, y, z);
					set_pixel(canvas, x, y, color);
				}
			}
		}
	}
}

f32 get_z_buffer_at(Canvas* z_buffer, i32 x, i32 y) {
	u32 cx = (u32)x;
	u32 cy = (u32)y;

	assert(
		cx < z_buffer->w &&
		cy < z_buffer->h &&
		z_buffer->framebuffer != nullptr
	);

	f32* result = (f32*)((byte*)z_buffer->framebuffer + cy * z_buffer->pitch + cx * sizeof(f32));
	return *result;
}

void set_z_buffer_at(Canvas* z_buffer, i32 x, i32 y, f32 val) {
	u32 cx = (u32)x;
	u32 cy = (u32)y;

	assert(
		cx < z_buffer->w &&
		cy < z_buffer->h &&
		z_buffer->framebuffer != nullptr
	);

	f32* z = (f32*)((byte*)z_buffer->framebuffer + cy * z_buffer->pitch + cx * sizeof(f32));
	*z = val;
}

f32 signed_triangle_area2(Vector2i p1, Vector2i p2, Vector2i p3) {
	return (f32)((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));
}

void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, u32 color) {
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

void set_pixel(Canvas* canvas, i32 x, i32 y, u32 color) {
	u32 cx = (u32)x;
	u32 cy = (u32)y;

	assert(
		cx < canvas->w &&
		cy < canvas->h &&
		canvas->framebuffer != nullptr
	);

	u32 bpp = 4; // byte per pixel
	u32* pixel = (u32*)((byte*)canvas->framebuffer + cy * canvas->pitch + cx * bpp);

	*pixel = color;
}

u32 to_u32_color(Vector3 rbg) {
	u32 red = (u32)Math::clamp(rbg.r * 255.0f, 0.0f, 255.0f);
	u32 green = (u32)Math::clamp(rbg.g * 255.0f, 0.0f, 255.0f);
	u32 blue = (u32)Math::clamp(rbg.b * 255.0f, 0.0f, 255.0f);

	return red << 16 | green << 8 | blue;
}

#include <random>
u32 get_rand_color() {
	static std::mt19937 gen(std::random_device{}());
	static std::uniform_real_distribution<f32> dist(0.0f, 1.1f);

	f32 r = dist(gen);
	f32 g = dist(gen);
	f32 b = dist(gen);
	return to_u32_color(Vector3{ r, g, b });
}
