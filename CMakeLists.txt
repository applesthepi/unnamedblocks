cmake_minimum_required(VERSION 3.15)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(USE_FOLDERS ON)

project(UB)

# Add X11 if using linux
# TODO: Proper wayland support
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(LINUX True)
	message(STATUS "Compiling For Linux")

	find_package(X11 REQUIRED)
	find_package(TBB REQUIRED)# Might need to go in UNIX, ill check later
	
	list(APPEND EXTRA_LIBRARIES ${X11_LIBRARIES})
	list(APPEND EXTRA_INCLUDES ${X11_INCLUDE_DIRS})
else()
	set(LINUX False)
endif()

# set(CMAKE_BUILD_TYPE Debug)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)

# find_library(TCC_LIBRARY NAMES libtcc1-64 PATHS "${CMAKE_SOURCE_DIR}/tcc")
# find_path(TCC_INCLUDE NAMES libtcc.h PATHS "${CMAKE_SOURCE_DIR}/tcc")

find_path(STB_INCLUDE_DIRS "stb.h")
find_path(CPPTOML_INCLUDE_DIRS "cpptoml.h")


# Visual Studio
# set_property(TARGET UB PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")
# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT UB)

set(VER_CLIENT "0.0.0" CACHE STRING "Full Client Unnamed Blocks Version")
set(VER_SERVER "0.0.0" CACHE STRING "Server Unnamed Blocks Version")
set(VER_MOD_VIN "0.0.0" CACHE STRING "Mod Vin Version")

option(BETA "Enable beta features" OFF)
option(AVX "Enable AVX2 support. This is supported on Intel Haswell processors or newer and AMD Excavator processors and all Zen processors or newer" OFF)
option(MODS "Build mods" ON)

add_subdirectory("UB_Client")
add_subdirectory("UB_Cappuccino")
add_subdirectory("UB_Espresso")
add_subdirectory("UB_RHR")

if (MODS)
	add_subdirectory("mods/UB_ModVin")
endif()

# # Options
# if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
# 	# Default search in appdata
# 	set(RES_FOLDER_SEARCH_PATH {APPDATA}/unnamedblocks CACHE STRING "(currently nop) Search dir for the res folder. {VARNAME} is environment variable. Blank for working directory")
# 	set(MODS_FOLDER_SEARCH_PATH {APPDATA}/unnamedblocks CACHE STRING "(currently nop) Search dir for the mods folder. {VARNAME} is environment variable. Blank for working directory")
# elseif (UNIX)
# 	# Default search in .local/share in unix like oses
# 	set(RES_FOLDER_SEARCH_PATH {HOME}/.local/share/unnamedblocks CACHE STRING "(currently nop) Search dir for the res folder. {VARNAME} is environment variable. Blank for working directory")
# 	set(MODS_FOLDER_SEARCH_PATH {HOME}/.local/share/unnamedblocks CACHE STRING "(currently nop) Search dir for the mods folder. {VARNAME} is environment variable. Blank for working directory")
# else ()
# 	# Unknown platform, dont assume anything
# 	set(RES_FOLDER_SEARCH_PATH "" CACHE STRING "(currently nop) Search dir for the res folder {VARNAME} is environment variable. Blank for working directory")
# 	set(MODS_FOLDER_SEARCH_PATH "" CACHE STRING "(currently nop) Search dir for the mods folder {VARNAME} is environment variable. Blank for working directory")
# endif ()













# if(MSVC)
# 	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT unnamedblocks)
# 	set(CMAKE_PREFIX_PATH STRING "${CMAKE_SOURCE_DIR}/cmake")
# 	find_file(csfml-audio NAMES libcsfml-audio.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-network NAMES libcsfml-network.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-graphics NAMES libcsfml-graphics.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-system NAMES libcsfml-system.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-window NAMES libcsfml-window.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	
# 	find_file(csfml-audio-d NAMES libcsfml-audio-d.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-network-d NAMES libcsfml-network-d.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-graphics-d NAMES libcsfml-graphics-d.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-system-d NAMES libcsfml-system-d.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
# 	find_file(csfml-window-d NAMES libcsfml-window-d.a PATHS "${CMAKE_SOURCE_DIR}/cmake/lib" "${CMAKE_SOURCE_DIR}/csfml/lib/msvc")
	
# 	find_path(csfml_include NAMES SFML/Graphics.h PATHS "${CMAKE_SOURCE_DIR}/csfml/include")
# else()
# 	find_library(csfml-audio NAMES csfml-audio)
# 	find_library(csfml-network NAMES csfml-network)
# 	find_library(csfml-graphics NAMES csfml-graphics)
# 	find_library(csfml-system NAMES csfml-system)
# 	find_library(csfml-window NAMES csfml-window)
# endif()


# find_package(SFML REQUIRED COMPONENTS graphics system)
# find_package(GLEW REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(Threads REQUIRED)
# find_library(libtcc NAMES tcc libtcc PATHS ${CMAKE_PREFIX_PATH})

# find_package(glad CONFIG REQUIRED)
# find_package(glm CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED)
# find_package(imgui CONFIG REQUIRED)
# find_package(RapidJSON CONFIG REQUIRED)
# find_package(Boost REQUIRED COMPONENTS system coroutine regex)
# find_package(Vulkan REQUIRED)
# find_package(zstd CONFIG REQUIRED)

# find_path(tcc_include NAMES include/libtcc.h PATHS "${CMAKE_PREFIX_PATH}")
# find_path(STB_INCLUDE_DIRS "stb.h")
# find_path(CPPTOML_INCLUDE_DIRS "cpptoml.h")

# # Add X11 if using linux
# # TODO: Proper wayland support
# if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
# 	set(LINUX True)
# 	message(STATUS "Compiling For Linux")
# 	find_package(X11 REQUIRED)
# 	# Might need to go in UNIX, ill check later
# 	find_package(TBB REQUIRED)
# 	list(APPEND EXTRA_LIBRARIES ${X11_LIBRARIES})
# 	list(APPEND EXTRA_INCLUDES ${X11_INCLUDE_DIRS})
# endif()
# # Not really posix, just a term given to every os that follows some standards (so everything besides windows)
# if(UNIX)
# 	set(POSIX True)
# endif()
# set(UnnamedBlocks_SRC
# 	"${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src/main.cpp"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src/ModLoader.cpp"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src/ModLoader.h"
# 	)
# add_executable(unnamedblocks
# 	${UnnamedBlocks_SRC}
# 	)
# # Preserve folders in visual studio structure
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/UnnamedBlocks/src FILES ${UnnamedBlocks_SRC})

# configure_file(config.h.in config.h)

# set_property(TARGET unnamedblocks PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")
# # Set unnamedblocks to the startup project
# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT unnamedblocks)

# target_link_libraries(unnamedblocks PUBLIC
# 	RHR
# 	Cappuccino
# 	# sfml-graphics
# 	# sfml-system
# 	# sfml-window
# 	# sfml-network
# 	# optimized "${csfml-audio}"
#     # optimized "${csfml-network}"
#     # optimized "${csfml-graphics}"
#     # optimized "${csfml-system}"
#     # optimized "${csfml-window}"
# 	# debug "${csfml-audio-d}"
#     # debug "${csfml-network-d}"
#     # debug "${csfml-graphics-d}"
#     # debug "${csfml-system-d}"
#     # debug "${csfml-window-d}"
# 	"${GLEW_LIBRARIES}"
# 	# "${OPENGL_LIBRARIES}"
# 	"${CMAKE_DL_LIBS}"
# 	"${EXTRA_LIBRARIES}"
# 	Threads::Threads
# 	)

# target_include_directories(unnamedblocks PUBLIC
# 	"${PROJECT_BINARY_DIR}"
# 	"RHR"
# 	"RHR/include"
# 	"${GLEW_INCLUDE_DIRS}"
# 	"${OPENGL_INCLUDE_DIRS}"
# 	"${EXTRA_INCLUDE_DIRS}"
# 	"${csfml_include}"
# 	)

# # Options
# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
# 	# Default search in appdata
# 	set(RES_FOLDER_SEARCH_PATH {APPDATA}/unnamedblocks CACHE STRING "(currently nop) Search dir for the res folder. {VARNAME} is environment variable. Blank for working directory")
# 	set(MODS_FOLDER_SEARCH_PATH {APPDATA}/unnamedblocks CACHE STRING "(currently nop) Search dir for the mods folder. {VARNAME} is environment variable. Blank for working directory")
# elseif(UNIX AND NOT APPLE)
# 	# Default search in .local/share in unix like oses
# 	set(RES_FOLDER_SEARCH_PATH {HOME}/.local/share/unnamedblocks CACHE STRING "(currently nop) Search dir for the res folder. {VARNAME} is environment variable. Blank for working directory")
# 	set(MODS_FOLDER_SEARCH_PATH {HOME}/.local/share/unnamedblocks CACHE STRING "(currently nop) Search dir for the mods folder. {VARNAME} is environment variable. Blank for working directory")
# else()
# 	# Unknown platform, dont assume anything
# 	set(RES_FOLDER_SEARCH_PATH "" CACHE STRING "(currently nop) Search dir for the res folder {VARNAME} is environment variable. Blank for working directory")
# 	set(MODS_FOLDER_SEARCH_PATH "" CACHE STRING "(currently nop) Search dir for the mods folder {VARNAME} is environment variable. Blank for working directory")
# endif()

# # Copy res folder
# add_custom_command(
# 	TARGET unnamedblocks POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	"${CMAKE_SOURCE_DIR}/res"
# 	$<TARGET_FILE_DIR:unnamedblocks>/res)
# # Copy csfml
# add_custom_command(
# 	TARGET unnamedblocks POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	"${CMAKE_SOURCE_DIR}/csfml"
# 	$<TARGET_FILE_DIR:unnamedblocks>/csfml)
# # Create mods folder for the mods to copy themselves into later
# add_custom_command(
# 	TARGET unnamedblocks POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E make_directory
# 	$<TARGET_FILE_DIR:unnamedblocks>/mods)
# add_custom_command(
# 	TARGET unnamedblocks POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	"${CMAKE_SOURCE_DIR}/Cappuccino/include"
# 	$<TARGET_FILE_DIR:unnamedblocks>/Cappuccino/include)
# add_custom_command(
#     TARGET unnamedblocks POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory
#     $<TARGET_FILE_DIR:unnamedblocks>/Cappuccino/lib/)
# add_custom_command(
#     TARGET unnamedblocks POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#     $<TARGET_FILE:Cappuccino>
#     $<TARGET_FILE_DIR:unnamedblocks>/Cappuccino/lib/)
# # Copy dependencies on windows
# if(MSVC)
# 	add_custom_command(
# 		TARGET unnamedblocks POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory
# 		"${CMAKE_SOURCE_DIR}/tcc"
# 		$<TARGET_FILE_DIR:unnamedblocks>)
# 	add_custom_command(
# 		TARGET unnamedblocks POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy
# 		$<TARGET_FILE:sfml-system>
# 		$<TARGET_FILE:sfml-window>
# 		$<TARGET_FILE:sfml-graphics>
# 		$<TARGET_FILE:sfml-network>
# 		"${CMAKE_SOURCE_DIR}/cmake/bin/libtcc.dll"
# 		$<TARGET_FILE_DIR:unnamedblocks>)
# 	add_custom_command(
# 		TARGET unnamedblocks POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy
# 		$<TARGET_FILE:Cappuccino>
# 		$<TARGET_FILE_DIR:unnamedblocks>/libCappuccino.dll)
# 	add_custom_command(
# 		TARGET unnamedblocks POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory
# 		"${CMAKE_SOURCE_DIR}/csfml/bin"
# 		$<TARGET_FILE_DIR:unnamedblocks>)
# else()
# 	add_custom_command(
# 		TARGET unnamedblocks POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy
# 		$<TARGET_FILE:Cappuccino>
# 		$<TARGET_FILE_DIR:unnamedblocks>/libCappuccino.so)
# endif()
