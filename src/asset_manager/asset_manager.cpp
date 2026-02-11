#include "./asset_manager.hpp"

#include <Windows.h>

static wchar asset_dir_path[MAX_PATH * 4];
static wchar asset_file_path[MAX_PATH * 4];

void init_asset_manager(wchar* asset_dir_path);
