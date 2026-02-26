#include "./render.hpp"

#include "./camera.hpp"
#include "./draw.hpp"
#include "./shader.hpp"

#include "./../asset_manager/asset_manager.hpp"

#include <cstdlib>
#include <cstring>
#include <cfloat>

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
static Entity global_head_entity = {
	.position = Vector3{0, 0, -3.5f},
	.rotation = Vector3{0, 0, 0},
	.scale = Vector3{ 1, 1, 1},
	.model = Model{},
};

static Vector3 global_light_direction = Math::normalize(Vector3{ 1, 1, -1 });
static f32 global_shine_val = 1.0f;
static i32 global_render_choice = 0;
static bool global_is_smooth = false;
static bool global_has_texture = false;

// internal functions
static void draw_entity(Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity);

static void transformation_pipeline(Camera* camera, Entity* entity, Vector4* vba);
static void get_model_matrix(Matrix4* result, Vector3 position, Vector3 rotation, Vector3 scale);
static void get_view_matrix(Matrix4* result, Camera* camera);
static void  get_projection_matrix(Matrix4* result, Camera* camera);

static void draw_faces(Canvas* canvas, Canvas* z_buffer, Entity* entity, Vector4* vba);
static Vector3 to_ndc(Vector4 vertex);
static Vector3 to_screen(Canvas* canvas, Vector3 vertex);

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

	camera_process(&global_camera, &event, delta_time);

	switch (event.key) {
	case Key::SPACE: {
		entity = (entity == &global_diablo_entity) ? &global_head_entity : entity = &global_diablo_entity;
		break;
	}
	case Key::T: {
		++global_render_choice;
		global_render_choice = (global_render_choice > 3) ? 0 : global_render_choice;

		if (global_render_choice == 0) {
			global_is_smooth = false;
			global_has_texture = false;
		}
		else if (global_render_choice == 3) {
			global_is_smooth = true;
		}
		else if (global_render_choice == 4) {
			global_has_texture = true;
		}
		break;
	}
	default: {
		break;
	}
	}

	draw_entity(arena, canvas, &global_camera, entity);
}
#include <iostream>
static void draw_entity(Memory::Arena* arena, Canvas* canvas, Camera* camera, Entity* entity) {

	Memory::Arena_Snapshot arena_snapshot = Memory::arena_create_snapshot(arena);

	i64 vba_count = entity->model.vertices_count;
	Vector4* vba = (Vector4*)Memory::arena_push(arena, vba_count * sizeof(Vector4), alignof(Vector4));

	Canvas z_buffer;
	z_buffer.w = canvas->w;
	z_buffer.h = canvas->h;
	z_buffer.pitch = canvas->w * sizeof(f32);

	z_buffer.framebuffer = Memory::arena_push(arena, z_buffer.h * z_buffer.pitch, alignof(f32));
	std::fill((f32*)z_buffer.framebuffer, (f32*)z_buffer.framebuffer + z_buffer.w * z_buffer.h, FLT_MAX);

	transformation_pipeline(camera, entity, vba);
	draw_faces(canvas, &z_buffer, entity, vba);

	Memory::arena_restore(arena_snapshot);
}

static void transformation_pipeline(Camera* camera, Entity* entity, Vector4* vba) {

	Matrix4 model;
	get_model_matrix(&model, entity->position, entity->rotation, entity->scale);

	Matrix4 view;
	get_view_matrix(&view, camera);

	Matrix4 projection;
	get_projection_matrix(&projection, camera);

	Matrix4 MVP = projection * view * model;

	Vector3* vertices = entity->model.vertices;
	i64 vba_count = entity->model.vertices_count;
#pragma omp parallel for
	for (i64 i = 0; i < vba_count; ++i) {
		vba[i] = Vector4{ vertices[i].x, vertices[i].y, vertices[i].z, 1 };
		vba[i] = MVP * vba[i];
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

static void  get_projection_matrix(Matrix4* result, Camera* camera) {

	f32 near = camera->near;
	f32 far = camera->far;
	f32 frustum_x = tanf(camera->fov / 2.0f) * camera->near;
	f32 frustum_y = frustum_x / camera->aspect_ratio;

	*result = Matrix4{
		near / frustum_x, 0, 0, 0,
		0, near / frustum_y, 0, 0,
		0, 0, far / (far - near), 1,
		0, 0, -(near * far) / (far - near), 0
	};
}

static void draw_faces(Canvas* canvas, Canvas* z_buffer, Entity* entity, Vector4* vba) {
	i64 faces_count = entity->model.faces_count;
	Face* faces = entity->model.faces;
	Vector3 v1, v2, v3;
#pragma omp parallel for
	for (i64 i = 0; i < faces_count; ++i) {
		v1 = to_ndc(vba[faces[i].v_indices[0]]);
		v2 = to_ndc(vba[faces[i].v_indices[1]]);
		v3 = to_ndc(vba[faces[i].v_indices[2]]);

		bool is_valid_face =
			v1.z >= 0.0f && v1.z <= 1.0f &&
			v2.z >= 0.0f && v2.z <= 1.0f &&
			v3.z >= 0.0f && v3.z <= 1.0f &&

			v1.x >= -1.0f && v1.x < 1.0f &&
			v2.x >= -1.0f && v2.x < 1.0f &&
			v3.x >= -1.0f && v3.x < 1.0f &&

			v1.y >= -1.0f && v1.y < 1.0f &&
			v2.y >= -1.0f && v2.y < 1.0f &&
			v3.y >= -1.0f && v3.y < 1.0f;

		if (is_valid_face) {
			v1 = to_screen(canvas, v1);
			v2 = to_screen(canvas, v2);
			v3 = to_screen(canvas, v3);

			Triangle face;
			face.v1 = v1;
			face.v2 = v2;
			face.v3 = v3;

			face.n1 = entity->model.normals[faces[i].v_indices[0]];
			face.n2 = entity->model.normals[faces[i].v_indices[1]];
			face.n3 = entity->model.normals[faces[i].v_indices[2]];

			face.light_direction = global_light_direction;
			face.view_direction = global_camera.basis.forward;

			face.color = COLOR_WHITE;
			face.shine = global_shine_val;

			face.is_smooth = global_is_smooth;
			face.has_texture = global_has_texture;

			switch (global_render_choice) {
			case 0: {
				Vector2i p1, p2, p3;

				p1.x = (i32)v1.x;
				p1.y = (i32)v1.y;

				p2.x = (i32)v2.x;
				p2.y = (i32)v2.y;

				p3.x = (i32)v3.x;
				p3.y = (i32)v3.y;

				draw_line(canvas, p1, p2, to_u32_color(COLOR_RED));
				draw_line(canvas, p1, p3, to_u32_color(COLOR_RED));
				draw_line(canvas, p2, p3, to_u32_color(COLOR_RED));

				set_pixel(canvas, p1.x, p1.y, to_u32_color(COLOR_WHITE));
				set_pixel(canvas, p2.x, p2.y, to_u32_color(COLOR_WHITE));
				set_pixel(canvas, p3.x, p3.y, to_u32_color(COLOR_WHITE));
				break;
			}
			case 1: {
				face.color = random_colors[i % array_count(random_colors)];
				draw_filled_triangle(canvas, z_buffer, &face);
				break;
			}
			case 2: {
				Vector3 v1_ = entity->model.vertices[faces[i].v_indices[0]];
				Vector3 v2_ = entity->model.vertices[faces[i].v_indices[1]];
				Vector3 v3_ = entity->model.vertices[faces[i].v_indices[2]];

				Vector3 face_normal = Math::normalize(Math::cross(v2_ - v1_, v3_ - v1_));
				face.color = compute_fragment(
					face_normal,
					face.light_direction,
					face.view_direction,
					COLOR_WHITE,
					face.shine
				);

				draw_filled_triangle(canvas, z_buffer, &face);
				break;
			}
			case 3: {
				draw_filled_triangle(canvas, z_buffer, &face);
				break;
			}
			}
			/*
			if (draw_type == Draw_Type::FILLED) {
				Triangle face;
				face.v1 = v1;
				face.v2 = v2;
				face.v3 = v3;

				face.n1 = entity->model.normals[faces[i].v_indices[0]];
				face.n2 = entity->model.normals[faces[i].v_indices[1]];
				face.n3 = entity->model.normals[faces[i].v_indices[2]];

				face.color = COLOR_WHITE;

				if (global_color_choice == 0) {
					f32 rand_color = (f32)(i * i) / (entity->model.faces_count * entity->model.faces_count);
					face.color = Vector3{ rand_color, rand_color, rand_color };
				}
				else if (global_color_choice == 1) {
					Vector3 v1_ = entity->model.vertices[faces[i].v_indices[0]];
					Vector3 v2_ = entity->model.vertices[faces[i].v_indices[1]];
					Vector3 v3_ = entity->model.vertices[faces[i].v_indices[2]];

					Vector3 face_normal = Math::normalize(Math::cross(v2_ - v1_, v3_ - v1_));
					face.color = compute_fragment(
						face_normal,
						global_light_direction,
						global_camera.basis.forward,
						COLOR_WHITE,
						global_shine_val
					);
				}
				draw_filled_triangle(canvas, z_buffer, &face);
			}
			else {
				Vector2i p1, p2, p3;

				p1.x = (i32)v1.x;
				p1.y = (i32)v1.y;

				p2.x = (i32)v2.x;
				p2.y = (i32)v2.y;

				p3.x = (i32)v3.x;
				p3.y = (i32)v3.y;

				draw_line(canvas, p1, p2, to_u32_color(COLOR_RED));
				draw_line(canvas, p1, p3, to_u32_color(COLOR_RED));
				draw_line(canvas, p2, p3, to_u32_color(COLOR_RED));

				set_pixel(canvas, p1.x, p1.y, to_u32_color(COLOR_WHITE));
				set_pixel(canvas, p2.x, p2.y, to_u32_color(COLOR_WHITE));
				set_pixel(canvas, p3.x, p3.y, to_u32_color(COLOR_WHITE));
			}
				*/
		}
	}
}

static Vector3 to_ndc(Vector4 vertex) {
	bool is_valid =
		vertex.x >= -vertex.w && vertex.x <= vertex.w &&
		vertex.y >= -vertex.w && vertex.y <= vertex.w &&
		vertex.z >= 0 && vertex.z <= vertex.w;

	if (is_valid) {
		return Vector3{
			vertex.x / vertex.w,
			vertex.y / vertex.w,
			vertex.z / vertex.w,
		};
	}
	else {
		return Vector3{
			2.0f,
			2.0f,
			2.0f
		};
	}
}

static Vector3 to_screen(Canvas* canvas, Vector3 vertex) {
	return Vector3{
		(vertex.x + 1) * canvas->w / 2,
		(vertex.y + 1) * canvas->h / 2,
		vertex.z
	};
}



