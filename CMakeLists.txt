cmake_minimum_required(VERSION 3.10)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(USE_FOLDERS ON)


project(unnamedblocks)
if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT unnamedblocks)
	set(CMAKE_PREFIX_PATH STRING "${CMAKE_SOURCE_DIR}/cmake")
	find_library(csfml-audio NAMES csfml-audio PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	find_library(csfml-network NAMES csfml-network PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	find_library(csfml-graphics NAMES csfml-graphics PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	find_library(csfml-system NAMES csfml-system PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	find_library(csfml-window NAMES csfml-window PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	find_path(csfml_include NAMES SFML/Graphics.h PATHS "${CMAKE_SOURCE_DIR}/csfml/include")
endif()


find_package(SFML REQUIRED COMPONENTS graphics system)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_library(libtcc NAMES tcc libtcc PATHS ${CMAKE_PREFIX_PATH})
find_path(tcc_include NAMES include/libtcc.h PATHS "${CMAKE_PREFIX_PATH}")

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
set(UnnamedBlocks_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src/main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src/ModLoader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src/ModLoader.h"
	)
add_executable(unnamedblocks 
	${UnnamedBlocks_SRC}
	)
# Preserve folders in visual studio structure
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src FILES ${UnnamedBlocks_SRC})

configure_file(config.h.in config.h)

set_property(TARGET unnamedblocks PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")
# Set unnamedblocks to the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT unnamedblocks)

add_subdirectory(Cappuccino)
add_subdirectory(Espresso)
add_subdirectory(RHR)

target_link_libraries(unnamedblocks PUBLIC
	RHR 
	sfml-graphics
	sfml-system
	sfml-window
	"${csfml-audio}"
        "${csfml-network}"
        "${csfml-graphics}"
        "${csfml-system}"
        "${csfml-window}"
	"${GLEW_LIBRARIES}"
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
	"${OPENGL_INCLUDE_DIRS}"
	"${EXTRA_INCLUDE_DIRS}"
	"${csfml_include}"
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

set(VERSION_UNNAMED_BLOCKS "0.0" CACHE STRING "Version of Unnamed Blocks")

option(DISABLE_ALL_MOD_BUILDS "Disable building the set of vanilla mods." OFF)
if (NOT DISABLE_ALL_MOD_BUILDS)
	add_subdirectory(VanillaMods)
endif()

# Copy res folder
add_custom_command(
	TARGET unnamedblocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/res"
	$<TARGET_FILE_DIR:unnamedblocks>/res)
# Copy csfml
add_custom_command(
	TARGET unnamedblocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/csfml"
	$<TARGET_FILE_DIR:unnamedblocks>/csfml)
# Create mods folder for the mods to copy themselves into later
add_custom_command(
	TARGET unnamedblocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory
	$<TARGET_FILE_DIR:unnamedblocks>/mods)
add_custom_command(
	TARGET unnamedblocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Cappuccino/include"
	$<TARGET_FILE_DIR:unnamedblocks>/Cappuccino/include)
	
add_custom_command(
	TARGET unnamedblocks POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:Cappuccino>
	$<TARGET_FILE_DIR:unnamedblocks>/Cappuccino/lib/libCappuccino.a)
# Copy dependencies on windows
if(MSVC)
	add_custom_command(
		TARGET unnamedblocks POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/tcc"
		$<TARGET_FILE_DIR:unnamedblocks>)
	add_custom_command(
		TARGET unnamedblocks POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		$<TARGET_FILE:sfml-system>
		$<TARGET_FILE:sfml-window>
		$<TARGET_FILE:sfml-graphics>
		"${CMAKE_SOURCE_DIR}/cmake/bin/libtcc.dll"
		$<TARGET_FILE_DIR:unnamedblocks>)
endif()
