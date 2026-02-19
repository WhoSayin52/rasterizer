#include "./asset_manager.hpp"

#include "./../renderer/render.hpp"

#include <Windows.h>

// constants
constexpr i64 MAX_OBJ_LINE_SIZE = 1024;
constexpr i64 MODEL_FACES_TOKEN_COUNT = 10;

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
static bool extract_data(Model* model, Stream* stream);

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
			stream.curr = stream.start;
			result = extract_data(model, &stream);
		}
	}

	CloseHandle(file);
	Memory::arena_restore(memory_snapshot);

	return result;
}

// internal to Obj helpers
static char* get_line(char* string, i64 string_count, Stream* stream);
static i64 tokenize_string(char tokens[][MAX_OBJ_LINE_SIZE], i64 arr_count, char* str);

static bool allocate_model_resources(Renderer_Memory* memory, Model* model, Stream* stream) {

	i64 vertices_count = 0;
	i64 textures_count = 0;
	i64 normals_count = 0;
	i64 faces_count = 0;

	char string[MAX_OBJ_LINE_SIZE] = "\0";
	while (get_line(string, (i64)array_count(string), stream)) {

		char tokens[MODEL_FACES_TOKEN_COUNT][MAX_OBJ_LINE_SIZE];
		i64 token_count = tokenize_string(tokens, array_count(tokens), string);

		if (token_count == -1) {
			return false;
		}

		if (token_count > 0) {

			char* curr = tokens[0];
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
		&memory->permanent, sizeof(Face) * faces_count, alignof(Face)
	);

	return true;
}

static bool extract_data(Model* model, Stream* stream) {
	char string[MAX_OBJ_LINE_SIZE] = "\0";

	Vector3* curr_v = model->vertices;
	Vector3* curr_t = model->textures;
	Vector3* curr_n = model->normals;
	Face* curr_f = model->faces;

	while (get_line(string, sizeof(string), stream)) {

		char tokens[MODEL_FACES_TOKEN_COUNT][MAX_OBJ_LINE_SIZE];
		i64 token_count = tokenize_string(tokens, array_count(tokens), string);

		if (token_count == -1) {
			return false;
		}
		if (token_count == 0) {
			continue;
		}

		if (tokens[0][0] == 'v') {
			if (token_count != 4) {
				return false;
			}
			char* end1;
			char* end2;
			char* end3;

			Vector3 v = {
				strtof(tokens[1], &end1),
				strtof(tokens[2], &end2),
				strtof(tokens[3], &end3)
			};

			if (end1 == tokens[1] || end2 == tokens[2] || end3 == tokens[3]) {
				return false;
			}

			if (tokens[0][1] == '\0') {
				*curr_v = v;
				++curr_v;
			}
			else if (tokens[0][1] == 't') {
				*curr_t = v;
				++curr_t;
			}
			else if (tokens[0][1] == 'n') {
				*curr_n = v;
				++curr_n;
			}
			else {
				return false;
			}
		}
		else if (tokens[0][0] == 'f') {
			if (token_count != MODEL_FACES_TOKEN_COUNT || tokens[0][1] != '\0') {
				return false;
			}
			char* end1;
			char* end2;
			char* end3;

			for (i64 i = 0; i < 3; ++i) {
				curr_f->v_indices[i] = strtoll(tokens[1 + i * 3], &end1, 10) - 1;
				curr_f->t_indices[i] = strtoll(tokens[2 + i * 3], &end2, 10) - 1;
				curr_f->n_indices[i] = strtoll(tokens[3 + i * 3], &end3, 10) - 1;
				if (end1 == tokens[1] || end2 == tokens[2] || end3 == tokens[3]) {
					return false;
				}
			}
			++curr_f;
		}
	}
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

static i64 tokenize_string(char tokens[][MAX_OBJ_LINE_SIZE], i64 arr_count, char* str) {
	assert(str != nullptr);

	char* curr = str;
	i64 count = 0;
	for (; count < arr_count; ++count) {
		while (*curr == ' ' || *curr == '\t' || *curr == '/') ++curr;

		if (*curr == '\0') {
			break;
		}

		i64 string_index = 0;
		while (*curr != ' ' && *curr != '\t' && *curr != '/' && *curr != '\0') {
			tokens[count][string_index] = *curr;
			++curr;
			++string_index;
			if (string_index == sizeof(tokens[count])) {
				return -1;
			}
		}
		tokens[count][string_index] = '\0';
	}
	return count;
}

