#ifndef DRAW_HPP
#define DRAW_HPP

#include <core/core.hpp>

struct Canvas;

constexpr Vector3 COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
constexpr Vector3 COLOR_RED = { 1.0f, 0.0f, 0.0f };

extern const Vector3 random_colors[6];

struct Triangle {
	Vector3 v1, v2, v3;
	Vector3 n1, n2, n3;

	Vector3 light_direction;
	Vector3 view_direction;

	Vector3 color;
	f32 shine;

	bool is_smooth;
	bool has_texture;
};

f32 get_z_buffer_at(Canvas* z_buffer, i32 x, i32 y);
void set_z_buffer_at(Canvas* z_buffer, i32 x, i32 y, f32 val);

f32 signed_triangle_area2(Vector2i p1, Vector2i p2, Vector2i p3);

void draw_filled_triangle(Canvas* canvas, Canvas* z_buffer, Triangle* triangle);
void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, u32 color);
void set_pixel(Canvas* canvas, i32 x, i32 y, u32 color);

u32 to_u32_color(Vector3 rbg);
u32 get_rand_color();

#endif // DRAW_HPP
