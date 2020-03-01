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
find_package(Threads REQUIRED)

# Add X11 if using linux
# TODO: Proper wayland support
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(LINUX True)
	message(STATUS "Compiling For Linux")
	find_package(X11 REQUIRED)
	# Might need to go in UNIX, ill check later
	find_package(TBB REQUIRED)
	list(APPEND EXTRA_LIBRARIES ${X11_LIBRARIES})
	list(APPEND EXTRA_INCLUDES ${X11_INCLUDE_DIRS})
endif()
# Not really posix, just a term given to every os that follows some standards (so everything besides windows)
if(UNIX)
	set(POSIX True)
endif()

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

# Options
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# Default search in appdata
	set(RES_FOLDER_SEARCH_PATH {APPDATA}/unnamedblocks CACHE STRING "(currently nop) Search dir for the res folder. {VARNAME} is environment variable. Blank for working directory")
	set(MODS_FOLDER_SEARCH_PATH {APPDATA}/unnamedblocks CACHE STRING "(currently nop) Search dir for the mods folder. {VARNAME} is environment variable. Blank for working directory")
elseif(UNIX AND NOT APPLE)
	# Default search in .local/share in unix like oses
	set(RES_FOLDER_SEARCH_PATH {HOME}/.local/share/unnamedblocks CACHE STRING "(currently nop) Search dir for the res folder. {VARNAME} is environment variable. Blank for working directory")
	set(MODS_FOLDER_SEARCH_PATH {HOME}/.local/share/unnamedblocks CACHE STRING "(currently nop) Search dir for the mods folder. {VARNAME} is environment variable. Blank for working directory")
else()
	# Unknown platform, dont assume anything
	set(RES_FOLDER_SEARCH_PATH "" CACHE STRING "(currently nop) Search dir for the res folder {VARNAME} is environment variable. Blank for working directory")
	set(MODS_FOLDER_SEARCH_PATH "" CACHE STRING "(currently nop) Search dir for the mods folder {VARNAME} is environment variable. Blank for working directory")
endif()
option(DISABLE_ALL_MOD_BUILDS "Disable building the set of vanilla mods." OFF)
if (NOT DISABLE_ALL_MOD_BUILDS)
	add_subdirectory(VanillaMods)
endif()

# Copy res to the build dir
add_custom_command(TARGET unnamedblocks POST_BUILD COMMAND
	cp -r "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
