cmake_minimum_required(VERSION 3.15)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(USE_FOLDERS ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 12)
enable_language(C CXX ASM)
# SET(ASM_OPTIONS "-x assembler-with-cpp")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
          ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
            )
            ENDIF()

#set(LLVM_DIR "dependencies/llvm/llvm/build/lib/cmake/llvm")
#set(Clang_DIR "dependencies/llvm/llvm/build/lib/cmake/clang")
#find_package(LLVM REQUIRED CONFIG)
#find_package(Clang REQUIRED)

#set(CMAKE_MODULE_PATH
#	${CMAKE_MODULE_PATH}
#	"${LLVM_CMAKE_DIR}"
#	)

#include(AddLLVM)

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

macro(print_all_variables)
	message(STATUS "print_all_variables------------------------------------------{")
	get_cmake_property(_variableNames VARIABLES)
	foreach(_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
	message(STATUS "print_all_variables------------------------------------------}")
endmacro()

project(UB)

# Add X11 if using linux TODO: Proper wayland support
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(LINUX True)
	message(STATUS "Compiling For Linux")

	find_package(X11 REQUIRED)
	find_package(TBB REQUIRED) # Might need to go in UNIX, ill check later

	list(APPEND EXTRA_LIBRARIES ${X11_LIBRARIES})
	list(APPEND EXTRA_INCLUDES ${X11_INCLUDE_DIRS})

    add_compile_options(-msse2)
else()
	set(LINUX False)
endif()

# set(CMAKE_BUILD_TYPE Debug)

# find_package(glm CONFIG REQUIRED) find_package(glfw3 CONFIG REQUIRED) find_package(RapidJSON
# CONFIG REQUIRED)
find_package(Vulkan REQUIRED FATAL_ERROR)
#find_package(LLVM REQUIRED)
# find_package(ZSTD REQUIRED FATAL_ERROR) find_package(freetype CONFIG REQUIRED) find_package(glad
# CONFIG REQUIRED)

find_package(PkgConfig)
pkg_check_modules(ZSTD zstd IMPORTED_TARGET GLOBAL)

# find_library(TCC_LIBRARY NAMES libtcc1-64 PATHS "${CMAKE_SOURCE_DIR}/tcc") find_path(TCC_INCLUDE
# NAMES libtcc.h PATHS "${CMAKE_SOURCE_DIR}/tcc") set(TCC_LIBRARY
# "${CMAKE_SOURCE_DIR}/tcc/lib/libtcc") set(TCC_INCLUDE "${CMAKE_SOURCE_DIR}/tcc/libtcc")

# find_library(TCC_LIBRARY NAMES tcc libtcc PATHS "${CMAKE_SOURCE_DIR}/cmake/lib")

# set(CMAKE_INCLUDES "${CMAKE_SOURCE_DIR}/cmake/include") set(VULKAN_LIB_LIST, "vulkan-1")

# message("${TCC_LIBRARY}") message("${CMAKE_INCLUDES}")

# find_path(STB_INCLUDE_DIRS "stb.h") find_path(CPPTOML_INCLUDE_DIRS "cpptoml.h")

# Visual Studio set_property(TARGET UB PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
# "${CMAKE_BINARY_DIR}/Debug") set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
# VS_STARTUP_PROJECT UB)

set(VER_CLIENT
	"0.0.0"
	CACHE STRING "Full Client Unnamed Blocks Version")
set(VER_SERVER
	"0.0.0"
	CACHE STRING "Server Unnamed Blocks Version")
set(VER_MOD_VIN
	"0.0.0"
	CACHE STRING "Mod Vin Version")

option(BETA "Enable beta features" OFF)
option(
	AVX
	"Enable AVX2 support. This is supported on Intel Haswell processors or newer and AMD Excavator processors and all Zen processors or newer"
	OFF)
option(MODS "Build mods" ON)

set(UB_DEPENDENCIES_INCLUDE
	${CMAKE_SOURCE_DIR}/dependencies/zstd/lib
	${CMAKE_SOURCE_DIR}/dependencies/stb/include
	${CMAKE_SOURCE_DIR}/dependencies/rapidjson/include
	${CMAKE_SOURCE_DIR}/dependencies/imgui/include
	${CMAKE_SOURCE_DIR}/dependencies/glfw/include
	${CMAKE_SOURCE_DIR}/dependencies/glm
	${CMAKE_SOURCE_DIR}/dependencies/freetype/include
	${CMAKE_SOURCE_DIR}/dependencies/vma/include
	${Vulkan_INCLUDE_DIRS})

message(STATUS "VULKAN ------------- ${VULKAN_PATH}")

add_subdirectory("dependencies")

set(UB_DEPENDENCIES imgui glfw glm freetype VulkanMemoryAllocator)

if(LINUX)
	set(UB_DEPENDENCIES_LINK imgui glfw glm freetype Vulkan::Vulkan VulkanMemoryAllocator X11) # Link X11 for linux
else()
	set(UB_DEPENDENCIES_LINK imgui glfw glm freetype Vulkan::Vulkan VulkanMemoryAllocator imm32) # Link imm32 for windows
endif()

set(UB_INCLUDES
	# "${CMAKE_SOURCE_DIR}/ub_lungo/include"
	"${CMAKE_SOURCE_DIR}/ub_mocha/include"
	"${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
	"${CMAKE_SOURCE_DIR}/ub_espresso/include"
	"${CMAKE_SOURCE_DIR}/ub_latte/include"
	"${CMAKE_SOURCE_DIR}/ub_macchiato/include"
	)

#find_package(LLVM REQUIRED CONFIG)

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

#find_package(Clang REQUIRED)

# Add path to LLVM modules
#set(CMAKE_MODULE_PATH
#	${CMAKE_MODULE_PATH}
#	"${LLVM_CMAKE_DIR}"
#	)

#include(AddLLVM)

add_subdirectory("ub_client")
# add_subdirectory("ub_lungo")
add_subdirectory("ub_macchiato")
add_subdirectory("ub_mocha")
add_subdirectory("ub_latte")
add_subdirectory("ub_cappuccino")
add_subdirectory("ub_espresso")

if(MODS)
	add_subdirectory("mods")
endif()

# print_all_variables()
