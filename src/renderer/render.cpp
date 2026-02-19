#include "./render.hpp"

#include "./../asset_manager/asset_manager.hpp"

#include <cstdlib>

// internal consts
static constexpr Vector3 COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
static constexpr Vector3 COLOR_RED = { 1.0f, 0.0f, 0.0f };
static f32 CAMERA_SPEED = 10.0f;

// internal structs
struct Camera {
	Vector3 position;
	Vector3 rotation;
	Vector3 x_axis;
	Vector3 y_axis;
	Vector3 z_axis;
	f32 fov; // field of view in radians;
	f32 near; // distance between camera and viewport
	f32 far;
	f32 aspect_ratio;
	Vector2 viewport;
};

struct Entity {
	Vector3 position;
	Model model;
	f32 scale;
};

// static global vars
static Camera global_camera = {
	.position = Vector3{0, 0, 0},
	.rotation = Vector3{0, 0, 0},
	.x_axis = Vector3{1, 0, 0},
	.y_axis = Vector3{0, 1, 0},
	.z_axis = Vector3{0, 0, 1},

	.fov = 60.0f * Math::PI32 / 180.0f, // radians
	.near = 0.1f,
	.far = 1000.0f,
	.aspect_ratio = 16.0f / 9.0f,
	.viewport = {}
};
static Entity global_diablo_entity = {
	.position = Vector3{0, 0, 3.5f},
	.model = Model{},
	.scale = 1.0f
};
struct Entity global_head_entity = {
	.position = Vector3{0, 0, 3.5f},
	.model = Model{},
	.scale = 1.0f
};

// internal functions
static void draw_entity(Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity, Projection PROJ_TYPE);
static void draw_faces(Canvas* canvas, Entity* entity, Vector3* vba);
static void transformation_pipeline(Canvas* canvas, Camera* camera, Entity* entity, Vector3* vba, Projection PROJ_TYPE);
static Vector3 project_to_screen(Canvas* canvas, Camera* camera, Vector3 vertex, Projection PROJ_TYPE);
static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, u32 color);
static void set_pixel(Canvas* canvas, i32 x, i32 y, u32 color);
static u32 to_u32_color(Vector3 rbg);

static void camera_process(Camera* camera, Event event, f32 delta_time);


void init_renderer(Renderer_Memory* memory, wchar* path_to_assets) {
	set_asset_manager_path(path_to_assets);

	bool rc = load_model(memory, &global_diablo_entity.model, L"diablo3_pose.obj");
	rc |= load_model(memory, &global_head_entity.model, L"head.obj");

	if (rc == false) {
		exit(1);
	}
}

void render(Memory::Arena* arena, Canvas* canvas, Event event, f32 delta_time) {

	static Entity* entity = &global_diablo_entity;
	static Projection projection_type = Projection::PERSPECTIVE;

	if (event.key != Key::NONE) {
		camera_process(&global_camera, event, delta_time);
	}

	switch (event.key) {
	case Key::SPACE: {
		entity == &global_diablo_entity ?
			entity = &global_head_entity : entity = &global_diablo_entity;
		break;
	}
	case Key::P: {
		projection_type == Projection::PERSPECTIVE ?
			projection_type = Projection::ORTHOGRAPHIC : projection_type = Projection::PERSPECTIVE;
		break;
	}
	default: {
		break;
	}
	}

	draw_entity(arena, canvas, &global_camera, entity, projection_type);
}

static void draw_entity(Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity, Projection PROJ_TYPE) {

	camera->viewport.w = (2 * (tanf(camera->fov / 2.0f) * camera->near));
	camera->viewport.h = camera->viewport.w / camera->aspect_ratio;

	Memory::Arena_Snapshot arena_snapshot = Memory::arena_create_snapshot(arena);

	i64 vba_count = entity->model.vertices_count;
	Vector3* vba = (Vector3*)Memory::arena_push(arena, vba_count * sizeof(Vector3), alignof(Vector3));

	transformation_pipeline(canvas, camera, entity, vba, PROJ_TYPE);
	draw_faces(canvas, entity, vba);

	//local_to_camera_space(camera, entity, vba);
	//draw_triangles(canvas, camera, entity, vba);

	Memory::arena_restore(arena_snapshot);
}

static void draw_faces(Canvas* canvas, Entity* entity, Vector3* vba) {
	i64 faces_count = entity->model.faces_count;
	Face* faces = entity->model.faces;
	Vector3 v1, v2, v3;
	Vector2i p1, p2, p3;
#pragma omp parallel for
	for (i64 i = 0; i < faces_count; ++i) {
		v1 = vba[faces[i].v_indices[0]];
		v2 = vba[faces[i].v_indices[1]];
		v3 = vba[faces[i].v_indices[2]];

		bool is_valid_face =
			v1.z >= 0.0f && v1.z <= 1.0f &&
			v2.z >= 0.0f && v2.z <= 1.0f &&
			v3.z >= 0.0f && v3.z <= 1.0f &&
			v1.x >= -(f32)canvas->origin.x && v1.x < (f32)canvas->origin.x - 1 &&
			v2.x >= -(f32)canvas->origin.x && v2.x < (f32)canvas->origin.x - 1 &&
			v3.x >= -(f32)canvas->origin.x && v3.x < (f32)canvas->origin.x - 1 &&
			v1.y >= -(f32)canvas->origin.y && v1.y < (f32)canvas->origin.y - 1 &&
			v2.y >= -(f32)canvas->origin.y && v2.y < (f32)canvas->origin.y - 1 &&
			v3.y >= -(f32)canvas->origin.y && v3.y < (f32)canvas->origin.y - 1;

		if (is_valid_face) {
			p1.x = (i32)roundf(v1.x);
			p1.y = (i32)roundf(v1.y);

			p2.x = (i32)roundf(v2.x);
			p2.y = (i32)roundf(v2.y);

			p3.x = (i32)roundf(v3.x);
			p3.y = (i32)roundf(v3.y);

			draw_line(canvas, p1, p2, to_u32_color(COLOR_RED));
			draw_line(canvas, p1, p3, to_u32_color(COLOR_RED));
			draw_line(canvas, p2, p3, to_u32_color(COLOR_RED));

			set_pixel(canvas, p1.x, p1.y, to_u32_color(COLOR_WHITE));
			set_pixel(canvas, p2.x, p2.y, to_u32_color(COLOR_WHITE));
			set_pixel(canvas, p3.x, p3.y, to_u32_color(COLOR_WHITE));
		}
	}
}

static void transformation_pipeline(Canvas* canvas, Camera* camera, Entity* entity, Vector3* vba, Projection PROJ_TYPE) {

	f32 scale = PROJ_TYPE == Projection::ORTHOGRAPHIC ? entity->scale * 0.03f : entity->scale;

	Vector3 camera_relative_pos = entity->position - camera->position;

	Vector3* vertices = entity->model.vertices;
	i64 vba_count = entity->model.vertices_count;
#pragma omp parallel for
	for (i64 i = 0; i < vba_count; ++i) {
		vba[i] = vertices[i] * scale + camera_relative_pos;
		vba[i] = vba[i].x * camera->x_axis + vba[i].y * camera->y_axis + vba[i].z * camera->z_axis;
		vba[i] = project_to_screen(canvas, camera, vba[i], PROJ_TYPE);
	}
}

static Vector3 project_to_screen(Canvas* canvas, Camera* camera, Vector3 vertex, Projection PROJ_TYPE) {

	f32 x = vertex.x;
	f32 y = vertex.y;
	f32 z = vertex.z;
	f32 near = camera->near;
	f32 far = camera->far;


	if (PROJ_TYPE == Projection::PERSPECTIVE) {
		// perspective projection 
		// IMPORTANT ASSUMPTION all vertices are with in view so no clipping or clamping
		// normalizing vertex which by now is relative to the camera(camera.position is its origin)
		// and using the unit vector as a line and finding the intersection with the  viewport plane
		if (vertex.z != 0) {
			x = x * near / z;
			y = y * near / z;
		}
	}

	Vector2 viewport = camera->viewport;
	f32 left = -viewport.w / 2;
	f32 bottom = -viewport.h / 2;

	x = ((x - left) / viewport.w) * 2 - 1;
	y = ((y - bottom) / viewport.h) * 2 - 1;
	z = (z - near) / (far - near);

	x = x * canvas->origin.x;
	y = y * canvas->origin.y;

	return Vector3{ x, y, z };
}

static void draw_line(Canvas* canvas, Vector2i p0, Vector2i p1, u32 color) {
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

static void set_pixel(Canvas* canvas, i32 x, i32 y, u32 color) {
	u32 cx = x + canvas->origin.x;
	u32 cy = y + canvas->origin.y;

	assert(
		cx < canvas->w &&
		cy < canvas->h &&
		canvas->framebuffer != nullptr
	);

	u32 bpp = 4; // byte per pixel
	u32* pixel = (u32*)((byte*)canvas->framebuffer + cy * canvas->pitch + cx * bpp);

	*pixel = color;
}

static u32 to_u32_color(Vector3 rbg) {
	u32 red = (u32)Math::clamp(rbg.r * 255.0f, 0.0f, 255.0f);
	u32 green = (u32)Math::clamp(rbg.g * 255.0f, 0.0f, 255.0f);
	u32 blue = (u32)Math::clamp(rbg.b * 255.0f, 0.0f, 255.0f);

	return red << 16 | green << 8 | blue;
}

static void camera_process(Camera* camera, Event event, f32 delta_time) {
	Key key = event.key;

	switch (key) {
	case Key::W: {
		camera->position += camera->z_axis * (CAMERA_SPEED * delta_time);
		break;
	}
	case Key::S: {
		camera->position -= camera->z_axis * (CAMERA_SPEED * delta_time);
		break;
	}
	case Key::A: {
		camera->position -= camera->x_axis * (CAMERA_SPEED * delta_time);
		break;
	}
	case Key::D: {
		camera->position += camera->x_axis * (CAMERA_SPEED * delta_time);
		break;
	}
	}
}

/*
My first attempts

static void draw_triangles(Canvas* canvas, Camera* camera, Entity* entity, Vector3* vba);
static Vector3 project_to_viewport(Camera* camera, Vector3 vertex);
static void local_to_camera_space(Camera* camera, Entity* entity, Vector3* vba);
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
#pragma omp parallel for
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

		u32 color = to_u32_color(COLOR_RED);

		draw_line(canvas, p1, p2, color);
		draw_line(canvas, p1, p3, color);
		draw_line(canvas, p2, p3, color);
	}
}

static Vector3 project_to_viewport(Camera* camera, Vector3 vertex) {
	// perspective projection
	// IMPORTANT ASSUMPTION all vertices are with in view so no clipping or clamping
	// normalizing vertex which by now is relative to the camera(camera.position is its origin)
	// and using the unit vector as a line and finding the intersection with the  viewport plane
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

*/
