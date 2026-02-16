#include "./asset_manager.hpp"

#include "./../render/render.hpp"

#include <Windows.h>

// global static vars
static wchar global_path_to_assets[MAX_PATH * 4];

// internal helper structs and functions for reading .obj files
struct Stream {
	char* start;
	char* end;
	char* curr;
	usize size;
	bool error;
};
static void init_stream(Stream* stream, void* buffer, usize size);
static bool allocate_model_resources(Renderer_Memory* memory, Model* model, Stream* stream);

void set_asset_manager_path(wchar* path_to_assets) {
	wcscpy_s(global_path_to_assets, array_count(global_path_to_assets), path_to_assets);
}

bool load_model(Renderer_Memory* memory, Model* model, const wchar* file_name) {
	wchar full_path[MAX_PATH * 4];
	wcscpy_s(full_path, array_count(full_path), global_path_to_assets);
	wcscat_s(full_path, array_count(full_path), file_name);

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

	void* file_buffer = (char*)Memory::arena_push(&memory->transient, file_size, alignof(byte));
	DWORD bytes_read;

	bool result = ReadFile(file, file_buffer, file_size, &bytes_read, nullptr);
	if (result != false) {
		Stream stream;
		init_stream(&stream, file_buffer, bytes_read);
		result = allocate_model_resources(memory, model, &stream);
		if (result) {
			// TODO: extract data from stream into models;
		}
	}

	CloseHandle(file);
	Memory::arena_restore(memory_snapshot);

	return result;
}


// internal to Obj helpers
static char* get_line(char* string, i64 string_count, Stream* stream);
static i64 tokenize_string(char* str);

static bool allocate_model_resources(Renderer_Memory* memory, Model* model, Stream* stream) {

	i64 vertices_count = 0;
	i64 textures_count = 0;
	i64 normals_count = 0;
	i64 faces_count = 0;

	char string[1024] = "\0";
	while (get_line(string, (i64)array_count(string), stream)) {

		i64 token_count = tokenize_string(string);
		if (token_count > 0) {

			char* curr = string;
			while (*curr == '\0') ++curr;

			if (curr[0] == 'v') {
				if (curr[1] == '\0') {
					++vertices_count;
				}
				else if (curr[1] == 't') {
					++textures_count;
				}
				else if (curr[1] == 'n') {
					++normals_count;
				}
			}
			else if (curr[0] == 'f') {
				if (curr[1] == '\0') {
					++faces_count;
				}
			}
		}
	}

	if (stream->error) {
		return false;
	}

	model->vertices_count = vertices_count;
	model->vertices = (Vector3*)Memory::arena_push(
		&memory->permanent, sizeof(Vector3) * vertices_count, alignof(Vector3)
	);
	model->textures_count = textures_count;
	model->textures = (Vector3*)Memory::arena_push(
		&memory->permanent, sizeof(Vector3) * textures_count, alignof(Vector3)
	);
	model->normals_count = normals_count;
	model->normals = (Vector3*)Memory::arena_push(
		&memory->permanent, sizeof(Vector3) * normals_count, alignof(Vector3)
	);
	model->faces_count = faces_count;
	model->faces = (Face*)Memory::arena_push(
		&memory->permanent, sizeof(Face) * vertices_count, alignof(Face)
	);

	return true;
}

static void init_stream(Stream* stream, void* buffer, usize size) {
	char* b = (char*)buffer;
	stream->start = b;
	stream->end = b + size;
	stream->curr = b;
	stream->size = size;
	stream->error = false;
}


static char* get_line(char* string, i64 string_count, Stream* stream) {
	assert(string != nullptr && stream != nullptr);
	char* end = stream->end;
	char* curr = stream->curr;
	char* prev = curr;
	i64 i = 0;
	for (; i < string_count - 1; ++i) {
		if (curr == end || *curr == '\n') {
			break;
		}
		string[i] = *curr;
		prev = curr;
		++curr;
	}

	if (*prev == '\r') {
		string[i - 1] = '\0';
	}

	stream->curr = curr == end ? end : curr + 1;

	if (*curr != '\n' && curr != end) {
		stream->error = true;
		string = nullptr;
	}
	else if (curr == end) {
		if (i == 0) {
			string = nullptr;
		}
		else {
			string[i + 1] = '\0';
		}
	}
	else {
		string[i] = '\0';
	}

	return string;
}

static i64 tokenize_string(char* str) {
	assert(str != nullptr);

	i64 count = 0;

	char* curr = str;
	char* prev = curr;
	while (*curr != '\0') {
		if (*curr == ' ' || *curr == '\t' || *curr == '/') {
			*curr = '\0';
			if (*prev != '\0') {
				++count;
			}
		}
		prev = curr;
		++curr;
	}
	if (*prev != '\0') ++count;
	return count;
}

