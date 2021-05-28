cmake_minimum_required(VERSION 3.15)
project(UB_CAPPUCCINO)

set(UB_CAPPUCCINO_SRC
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockActions.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlags.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlagsIterator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlock.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/ModBlockData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Logger.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataKey.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/PreProcessorData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Intrinsics.cpp"
	# Headers
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockActions.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlags.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlagsIterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlock.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/CappuccinoMod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/ModBlockData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Logger.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataKey.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/PreProcessorData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Intrinsics.h"
)

# Preserve folders in visual studio structure
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" FILES ${UB_CAPPUCCINO_SRC})

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_library(UB_CAPPUCCINO
	${UB_CAPPUCCINO_SRC}
)

# add_dependencies(UB_CAPPUCCINO

# )

target_include_directories(UB_CAPPUCCINO SYSTEM PUBLIC
	${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

target_include_directories(UB_CAPPUCCINO PUBLIC
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(UB_CAPPUCCINO PUBLIC
	glad::glad
	glm
	glfw
	imgui::imgui
	Vulkan::Vulkan
)

set_target_properties(UB_CAPPUCCINO PROPERTIES PREFIX "lib")

add_custom_command(
	TARGET UB_CAPPUCCINO POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E touch
	"${CMAKE_SOURCE_DIR}/UB_Client/src/main.cpp"
)

configure_file(config.h.in config.h)