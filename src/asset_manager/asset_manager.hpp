#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <core/core.hpp>

struct Renderer_Memory;

struct Face {
	i64 v_indices[3];
	i64 t_indices[3];
	i64 n_indices[3];
};

struct Model {
	Vector3* vertices;
	Vector3* textures;
	Vector3* normals;
	Face* faces;

	i64 vertices_count;
	i64 textures_count;
	i64 normals_count;
	i64 faces_count;
};

void set_asset_manager_path(wchar* path_to_assets);
bool load_model(Renderer_Memory* memory, Model* model, const wchar* file_name);

#endif // ASSET_MANAGER_HPP
