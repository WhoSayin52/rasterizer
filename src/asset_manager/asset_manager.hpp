#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include "./tgaimage.hpp"

#include <core/core.hpp>

struct Renderer_Memory;

struct Face {
	i64 v_indices[3];
	i64 t_indices[3];
	i64 n_indices[3];
};

struct Model {
	TGAImage normal_map;
	TGAImage diffuse_map;
	TGAImage specular_map;
	TGAImage glow_map;
	Vector3* vertices;
	Vector3* textures;
	Vector3* normals;
	Face* faces;

	i64 vertices_count;
	i64 textures_count;
	i64 normals_count;
	i64 faces_count;

	bool has_glow;
};

struct Entity {
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Model model;
};

void init_asset_manager(wchar* path_to_assets);
bool load_model(Renderer_Memory* memory, Model* model, const wchar* model_name);

#endif // ASSET_MANAGER_HPP
