target_sources(
	${EXE} PRIVATE 

	main.cpp
	asset_manager/asset_manager.hpp asset_manager/asset_manager.cpp
	asset_manager/tgaimage.hpp asset_manager/tgaimage.cpp
	renderer/render.hpp renderer/render.cpp
)

add_subdirectory(core)
add_subdirectory(renderer)
