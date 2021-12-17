cmake_minimum_required(VERSION 3.15)
project(UB_CAPPUCCINO)

file (GLOB_RECURSE UB_CAPPUCCINO_SRC "include/*/*.cpp" "include/*.cpp")

#set(UB_CAPPUCCINO_SRC
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/block.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/block.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/actions.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/actions.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/flags.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/flags.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/flags_iterator.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/flags_iterator.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/data.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/data.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/pass.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/mod/pass.cpp"
#
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/data_key.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/data_key.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/data_registry.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/data_registry.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/cappuccino.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/cappuccino.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/execution_thread.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/execution_thread.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/intrinsics.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/intrinsics.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/logger.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/logger.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/preprocessor_data.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/preprocessor_data.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/registration.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/registration.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/utils.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/utils.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/color.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/color.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/export.hpp"
#
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/cutils.h"
#)

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_library(UB_CAPPUCCINO STATIC
	${UB_CAPPUCCINO_SRC}
)
#add_library(UB_CAPPUCCINO_TCC SHARED
#	${UB_CAPPUCCINO_SRC}
#)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino FILES ${UB_CAPPUCCINO_SRC})
	target_compile_options(UB_CAPPUCCINO PRIVATE "/MP")
endif()

add_dependencies(UB_CLIENT
	UB_IMGUI
	)

target_link_libraries(UB_CAPPUCCINO PUBLIC
	UB_IMGUI

	glad::glad
	glfw
	Vulkan::Vulkan
	freetype

	#imm32.lib
)

target_include_directories(UB_CAPPUCCINO PUBLIC SYSTEM
	${PROJECT_BINARY_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/ub_imgui"
	"${CMAKE_SOURCE_DIR}/ub_imgui/backend"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIR}
	${CMAKE_INCLUDES}
)

# TODO: what is this for?
set_target_properties(UB_CAPPUCCINO PROPERTIES PREFIX "lib")
