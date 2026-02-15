#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <core/core.hpp>

struct Renderer_Memory;

struct Face {
	Vector3i vertex;
	Vector3i normal;
	Vector3i texture;
};

struct Model {
	Vector3* vertices;
	Vector3* normals;
	Vector3* textures;
	Face* faces;
};

void set_asset_manager_path(wchar* path_to_assets);
//bool load_model(Renderer_Memory* memory, Model* model, char* path);

#endif // ASSET_MANAGER_HPP
