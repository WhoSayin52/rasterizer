#include "./render.hpp"

#include "./../asset_manager/asset_manager.hpp"

#include <cstdlib>

// internal consts
static constexpr Vector3 COLOR_WHITE = { 1.0f, 1.0f, 1.0f };

// internal structs
struct Camera {
	Vector3 position;
	Vector3 right;
	Vector3 up;
	Vector3 forward;
	f32 fov; // field of view in radians;
	f32 near; // distance between camera and viewport
	f32 aspect_ratio;
};

struct Entity {
	Vector3 position;
	Model model;
};

// static global vars
static Camera global_camera = {
	.position = Vector3{0, 0, 0},
	.right = Vector3{1, 0, 0},
	.up = Vector3{0, 1, 0},
	.forward = Vector3{0, 0, 1},

	.fov = 60.0f * Math::PI32 / 180.0f, // radians
	.near = 1.0f, // 1 for simplicity 
	.aspect_ratio = 16.0f / 9.0f
};
static Entity global_diablo_entity = {
	.position = Vector3{0, 0, 3.5f},
	.model = Model{}
};

// internal functions
static void draw_entity(Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity);
static void draw_triangles(Canvas* canvas, Camera* camera, Entity* entity, Vector3* vba);
static Vector3 project_to_viewport(Camera* camera, Vector3 vertex);
static void local_to_camera_space(Camera* camera, Entity* entity, Vector3* vba);
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
	draw_entity(arena, canvas, &global_camera, &global_diablo_entity);
}

static void draw_entity(Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity) {

	Memory::Arena_Snapshot arena_snapshot = Memory::arena_create_snapshot(arena);

	i64 diablo_vba_count = global_diablo_entity.model.vertices_count;
	Vector3* diablo_vba = (Vector3*)Memory::arena_push(arena, diablo_vba_count * sizeof(Vector3), alignof(Vector3));

	local_to_camera_space(camera, &global_diablo_entity, diablo_vba);
	draw_triangles(canvas, camera, entity, diablo_vba);

	Memory::arena_restore(arena_snapshot);
}

static void draw_triangles(Canvas* canvas, Camera* camera, Entity* entity, Vector3* vba) {
	Vector2 viewport;
	viewport.w = (2 * (tanf(camera->fov / 2.0f) * camera->near));
	viewport.h = viewport.w / camera->aspect_ratio;

	Vector2 canvas_viewport_ratio;
	canvas_viewport_ratio.x = canvas->w / viewport.w;
	canvas_viewport_ratio.y = canvas->h / viewport.h;

	Vector3 v1, v2, v3;
	Vector2i p1, p2, p3;
	Face* faces = entity->model.faces;
	i64 faces_count = entity->model.faces_count;
	for (i64 i = 0; i < faces_count; ++i) {
		v1 = project_to_viewport(camera, vba[faces[i].v_indices[0]]);
		v2 = project_to_viewport(camera, vba[faces[i].v_indices[1]]);
		v3 = project_to_viewport(camera, vba[faces[i].v_indices[2]]);

		p1.x = (i32)roundf(v1.x * canvas_viewport_ratio.x);
		p1.y = (i32)roundf(v1.y * canvas_viewport_ratio.y);

		p2.x = (i32)roundf(v2.x * canvas_viewport_ratio.x);
		p2.y = (i32)roundf(v2.y * canvas_viewport_ratio.y);

		p3.x = (i32)roundf(v3.x * canvas_viewport_ratio.x);
		p3.y = (i32)roundf(v3.y * canvas_viewport_ratio.y);

		draw_line(canvas, p1, p2, COLOR_WHITE);
		draw_line(canvas, p1, p3, COLOR_WHITE);
		draw_line(canvas, p2, p3, COLOR_WHITE);
	}
}

static Vector3 project_to_viewport(Camera* camera, Vector3 vertex) {
	Vector3 vn = Math::normalize(vertex);

	f32 t = camera->near / vn.z;
	Vector3 result;
	result.x = vn.x * t;
	result.y = vn.y * t;
	result.z = vertex.z - camera->near;

	return result;
}

static void local_to_camera_space(Camera* camera, Entity* entity, Vector3* vba) {

	i64 count = entity->model.vertices_count;
	Vector3* vertices = entity->model.vertices;

	Vector3 camera_relative_position = entity->position - camera->position;
	for (i64 i = 0; i < count; ++i) {
		vba[i] = vertices[i] + camera_relative_position;
	}
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


