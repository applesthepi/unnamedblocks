cmake_minimum_required(VERSION 3.14)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(unnamedblocks VERSION 0.8)

find_package(SFML REQUIRED COMPONENTS graphics system)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

if (UNIX AND NOT APPLE)
	set(LINUX True)
	set(POSIX True)
	find_package(X11 REQUIRED)
	message("Using linux or similar")
	list(APPEND EXTRA_LIBRARIES ${X11_LIBRARIES})
	list(APPEND EXTRA_INCLUDES ${X11_INCLUDE_DIRS})
endif()
# NOTE: Apple is not supported, this is to give apple a small chance of
# compiling. No promises
if (APPLE)
	set(POSIX True)
endif()
option(DISABLE_ALL_MOD_BUILDS "Disable building the set of vanilla mods." OFF)
add_executable(unnamedblocks "UnnamedBlocks/src/main.cpp" "UnnamedBlocks/src/ModLoader.cpp")
configure_file(config.h.in config.h)
add_subdirectory(RHR)

target_link_libraries(unnamedblocks PUBLIC
	RHR 
	sfml-graphics
	sfml-system
	sfml-window
	"${GLEW_LIBRARIES}"
	"${Boost_LIBRARIES}"
	"${OPENGL_LIBRARIES}"
	"${CMAKE_DL_LIBS}"
	"${EXTRA_LIBRARIES}"
	Threads::Threads
	)

target_include_directories(unnamedblocks PUBLIC
	"${PROJECT_BINARY_DIR}"
	"RHR"
	"RHR/include"
	"${GLEW_INCLUDE_DIRS}"
	"${Boost_INCLUDE_DIRS}"
	"${OPENGL_INCLUDE_DIRS}"
	"${EXTRA_INCLUDE_DIRS}"
	)
if (NOT DISABLE_ALL_MOD_BUILDS)
	add_subdirectory(VanillaMods)
endif()

# Copy res to the build dir
add_custom_command(TARGET unnamedblocks POST_BUILD COMMAND
	cp -r "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
