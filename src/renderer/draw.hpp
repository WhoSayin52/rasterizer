#ifndef DRAW_HPP
#define DRAW_HPP

#include <core/core.hpp>

struct Canvas;

constexpr Vector3 COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
constexpr Vector3 COLOR_RED = { 1.0f, 0.0f, 0.0f };


f32 get_z_buffer_at(Canvas* z_buffer, i32 x, i32 y);
void set_z_buffer_at(Canvas* z_buffer, i32 x, i32 y, f32 val);

f32 signed_triangle_area2(Vector2i p1, Vector2i p2, Vector2i p3);

void draw_filled_triangle(Canvas* canvas, Canvas* z_buffer, Vector3 v1, Vector3 v2, Vector3 v3, u32 color);
void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, u32 color);
void set_pixel(Canvas* canvas, i32 x, i32 y, u32 color);

u32 to_u32_color(Vector3 rbg);
u32 get_rand_color();

#endif // DRAW_HPP
