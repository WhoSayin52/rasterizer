target_sources(
	${EXE} PRIVATE 

	main.cpp
	asset_manager/asset_manager.hpp asset_manager/asset_manager.cpp
	renderer/render.hpp renderer/render.cpp
)

add_subdirectory(core)
add_subdirectory(renderer)
