#include "./asset_manager.hpp"

#include "./../render/render.hpp"

#include <Windows.h>

static wchar global_path_to_assets[MAX_PATH * 4];

void set_asset_manager_path(wchar* path_to_assets) {
	wcscpy_s(global_path_to_assets, array_count(global_path_to_assets), path_to_assets);
}

bool load_model(Renderer_Memory* memory, Model* model, const wchar* file_name) {
	wchar full_path[MAX_PATH * 4];
	wcscpy_s(full_path, array_count(full_path), global_path_to_assets);
	wcscat_s(full_path, array_count(full_path), file_name);
	OutputDebugString(full_path);
	HANDLE file = CreateFile(
		full_path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr
	);
	if (file == INVALID_HANDLE_VALUE) {
		model->vertices = nullptr;
		model->normals = nullptr;
		model->textures = nullptr;
		model->faces = nullptr;
		return false;
	}

	DWORD file_size = GetFileSize(file, nullptr);
	assert(Memory::arena_remaining_size(&memory->transient, alignof(byte)) >= file_size);

	Memory::Arena_Snapshot memory_snapshot = Memory::arena_create_snapshot(&memory->transient);

	void* file_buffer = Memory::arena_push(&memory->transient, file_size, alignof(byte));

	DWORD bytes_read;
	bool result = ReadFile(file, file_buffer, file_size, &bytes_read, nullptr);

	Memory::arena_restore(memory_snapshot);

	CloseHandle(file);

	return result;
}
