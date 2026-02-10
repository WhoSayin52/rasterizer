cmake_minimum_required(VERSION 4.2)

project(rasterizer VERSION 0.1.0 LANGUAGES CXX)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/asset ${CMAKE_BINARY_DIR}/asset SYMBOLIC)

set(EXE rasterizer)
add_executable(${EXE})
add_subdirectory(src)

target_include_directories(
	${EXE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(${EXE} PRIVATE cxx_std_23)
target_compile_options(
	${EXE} PRIVATE
	/W4 /WX
	$<$<CONFIG:Debug>:/Od /Zi>
	$<$<CONFIG:Release>:/O2 /DNDEBUG>
)
target_compile_definitions(
	${EXE} PRIVATE
	UNICODE _UNICODE
)

set_target_properties(
	${EXE} PROPERTIES  
	WIN32_EXECUTABLE on
	DEBUG_POSTFIX "d"
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED on
	CXX_EXTENSIONS off
)
