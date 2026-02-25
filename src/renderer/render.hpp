#ifndef RENDER_HPP
#define RENDER_HPP

#include <core/core.hpp>

enum class Draw_Type {
	FILLED,
	WIREFRAME
};

enum class Key {
	NONE,
	SPACE,
	CONTROL,
	T,
	W,
	A,
	S,
	D,
};

struct Event {
	Key key;
	Vector2 mouse_position;
};

struct Renderer_Memory {
	Memory::Arena permanent;
	Memory::Arena transient;
};

struct Canvas {
	void* framebuffer;
	u32 w;
	u32 h;
	u32 pitch;
};

void init_renderer(Renderer_Memory* memory, wchar* path_to_assets);

void render(Memory::Arena* arena, Canvas* canvas, Event event, f32 delta_time);

#endif // RENDER_HPP
