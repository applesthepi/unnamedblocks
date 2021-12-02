cmake_minimum_required(VERSION 3.15)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(USE_FOLDERS ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 12)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

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

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

#find_library(TCC_LIBRARY NAMES libtcc1-64 PATHS "${CMAKE_SOURCE_DIR}/tcc")
#find_path(TCC_INCLUDE NAMES libtcc.h PATHS "${CMAKE_SOURCE_DIR}/tcc")
#set(TCC_LIBRARY "${CMAKE_SOURCE_DIR}/tcc/lib/libtcc")
#set(TCC_INCLUDE "${CMAKE_SOURCE_DIR}/tcc/libtcc")

find_library(TCC_LIBRARY NAMES tcc libtcc PATHS "${CMAKE_SOURCE_DIR}/cmake/lib")

set(CMAKE_INCLUDES "${CMAKE_SOURCE_DIR}/cmake/include")
set(VULKAN_LIB_LIST, "vulkan-1")

#message("${TCC_LIBRARY}")
#message("${CMAKE_INCLUDES}")

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

add_subdirectory("ub_client")
add_subdirectory("ub_cappuccino")
add_subdirectory("ub_espresso")
add_subdirectory("ub_rhr")

if (MODS)
	add_subdirectory("mods/ub_mod_vin")
endif()

########################################################################################
########################################################################################
########################################################################################

# IMGUI

add_library(imgui STATIC)
target_sources(imgui PRIVATE
		"vendor/imgui/imgui_demo.cpp"
		"vendor/imgui/imgui_draw.cpp"
		"vendor/imgui/imgui_tables.cpp"
		"vendor/imgui/imgui_widgets.cpp"
		"vendor/imgui/imgui.cpp"

		"vendor/imgui/backends/imgui_impl_vulkan.cpp"
		"vendor/imgui/backends/imgui_impl_glfw.cpp"
)

target_include_directories(imgui PUBLIC SYSTEM
		vendor/imgui
		vendor/imgui/backends
#		${GLFW_SOURCE_DIR}/include
		${Vulkan_INCLUDE_DIR}
		${CMAKE_INCLUDES}

		# TODO: fix this
		"vcpkg/installed/x64-windows/include"
		"vcpkg/installed/x64-linux/include"
)

# CLIP

#add_library(clip STATIC)
#target_sources(clip PRIVATE
#		"vendor/clip/clip.cpp"
#		"vendor/clip/clip_none.cpp"
#		"vendor/clip/clip_win.cpp"
##		"vendor/clip/clip_x11.cpp"
#		"vendor/clip/image.cpp"
#)
#
#target_include_directories(clip PUBLIC
#		"vendor/clip"
#)