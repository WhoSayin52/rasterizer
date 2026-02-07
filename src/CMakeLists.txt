target_sources(
	${EXE} PRIVATE 

	main.cpp
	render/render.hpp render/render.cpp
)

add_subdirectory(math)
