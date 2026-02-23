#include "./render.hpp"

#include "./camera.hpp"
#include "./draw.hpp"

#include "./../asset_manager/asset_manager.hpp"

#include <cstdlib>
#include <cstring>

// internal structs
struct Entity {
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Model model;
};

// static global vars
static Camera global_camera = {
	.position = Vector3{0, 0, 0},
	.rotation = Vector3{0, 180, 0},
	.basis = Matrix3{
		1, 0, 0, // col1
		0, 1, 0, // col2
		0, 0, 1	// col3
	},

	.fov = 60.0f * Math::PI32 / 180.0f, // radians
	.near = 0.1f,
	.far = 100.0f,
	.aspect_ratio = 16.0f / 9.0f,
	.viewport = {}
};

static Entity global_diablo_entity = {
	.position = Vector3{0, 0, -3.5f},
	.rotation = Vector3{0, 0, 0},
	.scale = Vector3{ 1, 1, 1},
	.model = Model{},
};
struct Entity global_head_entity = {
	.position = Vector3{0, 0, -3.5f},
	.rotation = Vector3{0, 0, 0},
	.scale = Vector3{ 1, 1, 1},
	.model = Model{},
};

// internal functions
static void draw_entity(
	Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity, Draw_Type draw_type, Projection proj_type
);
static void draw_faces(Canvas* canvas, Canvas* z_buffer, Entity* entity, Vector4* vba, Draw_Type draw_type);

static void transformation_pipeline(Canvas* canvas, Camera* camera, Entity* entity, Vector4* vba, Projection proj_type);
static void get_model_matrix(Matrix4* result, Vector3 position, Vector3 rotation, Vector3 scale);
static void get_view_matrix(Matrix4* result, Camera* camera);
static Vector4 project_to_screen(Canvas* canvas, Camera* camera, Vector4 vertex, Projection proj_type);

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
	static Draw_Type draw_type = Draw_Type::FILLED;
	static Projection projection_type = Projection::PERSPECTIVE;

	camera_process(&global_camera, &event, delta_time);

	switch (event.key) {
	case Key::SPACE: {
		entity = (entity == &global_diablo_entity) ? &global_head_entity : entity = &global_diablo_entity;
		break;
	}
	case Key::T: {
		draw_type = (draw_type == Draw_Type::FILLED) ? Draw_Type::WIREFRAME : Draw_Type::FILLED;
		break;
	}
	case Key::P: {
		projection_type = (projection_type == Projection::PERSPECTIVE) ?
			Projection::ORTHOGRAPHIC : projection_type = Projection::PERSPECTIVE;
		break;
	}
	default: {
		break;
	}
	}

	draw_entity(arena, canvas, &global_camera, entity, draw_type, projection_type);
}

static void draw_entity(
	Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity, Draw_Type draw_type, Projection proj_type
) {

	camera->viewport.w = (2 * (tanf(camera->fov / 2.0f) * camera->near));
	camera->viewport.h = camera->viewport.w / camera->aspect_ratio;

	Memory::Arena_Snapshot arena_snapshot = Memory::arena_create_snapshot(arena);

	i64 vba_count = entity->model.vertices_count;
	Vector4* vba = (Vector4*)Memory::arena_push(arena, vba_count * sizeof(Vector4), alignof(Vector4));

	Canvas z_buffer;
	z_buffer.w = canvas->w;
	z_buffer.h = canvas->h;
	z_buffer.pitch = canvas->w * sizeof(f32);
	z_buffer.origin = canvas->origin;

	z_buffer.framebuffer = Memory::arena_push(arena, z_buffer.h * z_buffer.pitch, alignof(f32));
	memset(z_buffer.framebuffer, INT8_MAX, z_buffer.h * z_buffer.pitch);

	transformation_pipeline(canvas, camera, entity, vba, proj_type);
	draw_faces(canvas, &z_buffer, entity, vba, draw_type);

	Memory::arena_restore(arena_snapshot);
}

static void draw_faces(Canvas* canvas, Canvas* z_buffer, Entity* entity, Vector4* vba, Draw_Type draw_type) {
	i64 faces_count = entity->model.faces_count;
	Face* faces = entity->model.faces;
	Vector4 v1, v2, v3;
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


			if (draw_type == Draw_Type::FILLED) {
				draw_filled_triangle(canvas, z_buffer, v1, v2, v3, (u32)(i * i));
			}
			else {
				Vector2i p1, p2, p3;

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
}

static void transformation_pipeline(Canvas* canvas, Camera* camera, Entity* entity, Vector4* vba, Projection proj_type) {

	Vector3 entity_scale = (proj_type == Projection::ORTHOGRAPHIC) ? entity->scale * 0.03f : entity->scale;

	Matrix4 model;
	get_model_matrix(&model, entity->position, entity->rotation, entity_scale);

	Matrix4 view;
	get_view_matrix(&view, camera);

	Matrix4 view_x_model = view * model;

	Vector3* vertices = entity->model.vertices;
	i64 vba_count = entity->model.vertices_count;
#pragma omp parallel for
	for (i64 i = 0; i < vba_count; ++i) {
		vba[i] = Vector4{ vertices[i].x, vertices[i].y, vertices[i].z, 1 };
		vba[i] = view_x_model * vba[i];
		vba[i] = project_to_screen(canvas, camera, vba[i], proj_type);
	}
}

static void get_model_matrix(Matrix4* result, Vector3 position, Vector3 rotation, Vector3 scale) {
	Matrix4 translation_matrix;
	translation_matrix.x_axis = Vector4{ 1, 0, 0, 0 };
	translation_matrix.y_axis = Vector4{ 0, 1, 0, 0 };
	translation_matrix.z_axis = Vector4{ 0, 0, 1, 0 };
	translation_matrix.w_axis = Vector4{ position.x, position.y, position.z, 1 };

	Matrix4 scale_matrix;
	scale_matrix.x_axis = Vector4{ scale.x, 0, 0, 0 };
	scale_matrix.y_axis = Vector4{ 0, scale.y, 0, 0 };
	scale_matrix.z_axis = Vector4{ 0, 0, scale.z, 0 };
	scale_matrix.w_axis = Vector4{ 0, 0, 0, 1 };

	f32 x_rad = Math::to_radians(rotation.x);
	f32 y_rad = Math::to_radians(rotation.y);
	f32 z_rad = Math::to_radians(rotation.z);

	Matrix4 rotation_matrix_around_x;
	rotation_matrix_around_x.x_axis = Vector4{ 1, 0, 0, 0 };
	rotation_matrix_around_x.y_axis = Vector4{ 0, cosf(x_rad), sinf(x_rad), 0 };
	rotation_matrix_around_x.z_axis = Vector4{ 0, -sinf(x_rad), cosf(x_rad), 0 };
	rotation_matrix_around_x.w_axis = Vector4{ 0, 0, 0, 1 };

	Matrix4 rotation_matrix_around_y;
	rotation_matrix_around_y.x_axis = Vector4{ cosf(y_rad), 0, -sinf(y_rad), 0 };
	rotation_matrix_around_y.y_axis = Vector4{ 0, 1, 0, 0 };
	rotation_matrix_around_y.z_axis = Vector4{ sinf(y_rad), 0, cosf(y_rad), 0 };
	rotation_matrix_around_y.w_axis = Vector4{ 0, 0, 0, 1 };

	Matrix4 rotation_matrix_around_z;
	rotation_matrix_around_z.x_axis = Vector4{ cosf(z_rad), -sinf(z_rad), 0, 0 };
	rotation_matrix_around_z.y_axis = Vector4{ sinf(z_rad), cosf(z_rad), 0, 0 };
	rotation_matrix_around_z.z_axis = Vector4{ 0, 0, 1, 0 };
	rotation_matrix_around_z.w_axis = Vector4{ 0, 0, 0, 1 };

	// x y z order
	Matrix4 rotation_matrix = rotation_matrix_around_z * rotation_matrix_around_y * rotation_matrix_around_x;

	*result = translation_matrix * rotation_matrix * scale_matrix;
}

static void get_view_matrix(Matrix4* result, Camera* camera) {
	Vector3 cam_pos = camera->position;

	Matrix4 translation;
	translation.x_axis = Vector4{ 1, 0, 0, 0 };
	translation.y_axis = Vector4{ 0, 1, 0, 0 };
	translation.z_axis = Vector4{ 0, 0, 1, 0 };
	translation.w_axis = Vector4{ -cam_pos.x, -cam_pos.y, -cam_pos.z, 1 };

	Matrix3 temp;
	Math::transpose(&camera->basis, &temp);

	Matrix4 inv_basis;
	inv_basis.x_axis = Vector4{ temp.n11, temp.n21, temp.n31, 0 };
	inv_basis.y_axis = Vector4{ temp.n12, temp.n22, temp.n32, 0 };
	inv_basis.z_axis = Vector4{ temp.n13, temp.n23, temp.n33, 0 };
	inv_basis.w_axis = Vector4{ 0, 0, 0, 1 };

	*result = inv_basis * translation;
}

static Vector4 project_to_screen(Canvas* canvas, Camera* camera, Vector4 vertex, Projection proj_type) {

	f32 x = vertex.x;
	f32 y = vertex.y;
	f32 z = vertex.z;
	f32 near = camera->near;
	f32 far = camera->far;

	if (proj_type == Projection::PERSPECTIVE) {
		// perspective projection 
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

	return Vector4{ x, y, z, 1 };
}






