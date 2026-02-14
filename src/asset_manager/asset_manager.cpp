#include "./asset_manager.hpp"

#include <Windows.h>

static wchar global_path_to_assets[MAX_PATH * 4];

void set_asset_manager_path(wchar* path_to_assets) {
	wcscpy_s(global_path_to_assets, array_count(global_path_to_assets), path_to_assets);
}
