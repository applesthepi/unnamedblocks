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

target_link_libraries(UB_CAPPUCCINO PUBLIC
	glad::glad
	glfw
#	imgui::imgui
	Vulkan::Vulkan
	freetype
)

#target_link_libraries(UB_CAPPUCCINO_TCC PUBLIC
#		glad::glad
#		glfw
#		imgui::imgui
#		Vulkan::Vulkan
#		freetype
#		${TCC_LIBRARY}
#		)

target_include_directories(UB_CAPPUCCINO PUBLIC
	${PROJECT_BINARY_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
	${CMAKE_INCLUDES}
)

#target_include_directories(UB_CAPPUCCINO_TCC PUBLIC
#		${PROJECT_BINARY_DIR}
#		"${CMAKE_CURRENT_SOURCE_DIR}/include"
#
#		${STB_INCLUDE_DIRS}
#		${RAPIDJSON_INCLUDE_DIRS}
#		${VULKAN_INCLUDE_DIRS}
#		${CMAKE_INCLUDES}
#		)

# TODO: what is this for?
set_target_properties(UB_CAPPUCCINO PROPERTIES PREFIX "lib")

#add_custom_command(
#	TARGET UB_CAPPUCCINO POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E touch
#	"${CMAKE_SOURCE_DIR}/UB_Client/src/main.cpp"
#)


#configure_file (
#		"${PROJECT_SOURCE_DIR}/config.h.in"
#		"${PROJECT_BINARY_DIR}/cap_config.h"
#)