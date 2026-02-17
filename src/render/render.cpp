#include "./render.hpp"

#include "./../asset_manager/asset_manager.hpp"

#include <cstdlib>

// internal structs
struct Basis {
	Vector3 x;
	Vector3 y;
	Vector3 z;
};
struct Camera {
	Vector3 position;
	Basis basis;
	f32 fov; // field of view;
	f32 near; // distance between camera and viewport
};

struct Entity {
	Vector3 position;
	Model model;
};

// static global vars
static Camera camera = {
	.position = Vector3{0, 0, 0},
	.basis = Basis{
		.x = Vector3{1, 0, 0},
		.y = Vector3{0, 1, 0},
		.z = Vector3{0, 0, 1}
	},
	.fov = 60.0f, // degrees
	.near = 1.0f // 1 for simplicity 
};
static Entity global_diablo_entity = {
	.position = Vector3{0, 0, 1000},
	.model = Model{}
};

// internal functions
static void to_camera_space(Camera* camera, Vector3* vertices, i64 vertices_count);
static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, Vector3 color);
static void set_pixel(Canvas* canvas, i32 x, i32 y, Vector3 color);

void init_renderer(Renderer_Memory* memory, wchar* path_to_assets) {
	set_asset_manager_path(path_to_assets);

	bool rc = load_model(memory, &global_diablo_entity.model, L"diablo3_pose.obj");

	if (rc == false) {
		exit(1);
	}
}

void render(Memory::Arena* arena, Canvas* canvas) {

	Memory::Arena_Snapshot arena_snapshot = Memory::arena_create_snapshot(arena);

	i64 vertices_count = global_diablo_entity.model.vertices_count;
	Vector3* vertices = (Vector3*)Memory::arena_push(arena, vertices_count * sizeof(Vector3), alignof(Vector3));



	Memory::arena_restore(arena_snapshot);
}

static void to_camera_space(Camera* camera, Vector3* vertices, i64 vertices_count, Entity* entity){
	
	//Vector3* vertex = entity
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

	u32 red = (u32)Math::clamp(color.r * 255.0f, 0.0f, 255.0f);
	u32 green = (u32)Math::clamp(color.g * 255.0f, 0.0f, 255.0f);
	u32 blue = (u32)Math::clamp(color.b * 255.0f, 0.0f, 255.0f);

	u32* pixel = (u32*)((byte*)canvas->memory + cy * canvas->pitch + cx * bpp);

	*pixel = red << 16 | green << 8 | blue;
}


