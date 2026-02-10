target_sources(
	${EXE} PRIVATE 

	main.cpp
	asset_manager/asset_manager.hpp asset_manager/asset_manager.cpp
	render/render.hpp render/render.cpp
)

add_subdirectory(core)
