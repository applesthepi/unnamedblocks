cmake_minimum_required(VERSION 3.15)
project(UB_CAPPUCCINO)

set(UB_CAPPUCCINO_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlock.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlock.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlockActions.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlockActions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlockFlags.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlockFlags.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlockFlagsIterator.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/block/ModBlockFlagsIterator.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/runtime/DataKey.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/runtime/DataKey.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/runtime/DataRegistry.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/runtime/DataRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/runtime/ModBlockData.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/runtime/ModBlockData.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/cappuccino.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/cappuccino.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/ExecutionThread.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/ExecutionThread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Intrinsics.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Intrinsics.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Logger.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Logger.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/ModBlockPass.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/ModBlockPass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/PreProcessorData.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/PreProcessorData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Registration.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Registration.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Utils.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Utils.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Color.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Color.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino/Export.hpp"
)

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_library(UB_CAPPUCCINO
	${UB_CAPPUCCINO_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino FILES ${UB_CAPPUCCINO_SRC})
	target_compile_options(UB_CAPPUCCINO PRIVATE "/MP")
endif()

target_link_libraries(UB_CAPPUCCINO PUBLIC
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
)

target_include_directories(UB_CAPPUCCINO PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

# TODO: what is this for?
set_target_properties(UB_CAPPUCCINO PROPERTIES PREFIX "lib")

#add_custom_command(
#	TARGET UB_CAPPUCCINO POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E touch
#	"${CMAKE_SOURCE_DIR}/UB_Client/src/main.cpp"
#)

configure_file(config.h.in config.h)