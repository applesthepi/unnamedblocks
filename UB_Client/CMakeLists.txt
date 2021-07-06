cmake_minimum_required(VERSION 3.15)
project(UB_CLIENT)

set(UB_CLIENT_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/ModLoader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ModLoader.h"
)

# Preserve folders in visual studio structure
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${UB_CLIENT_SRC})

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_executable(UB_CLIENT
	${UB_CLIENT_SRC}
)

add_dependencies(UB_CLIENT
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

target_include_directories(UB_CLIENT SYSTEM PUBLIC
	${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

target_include_directories(UB_CLIENT PUBLIC
    "${CMAKE_SOURCE_DIR}/UB_Cappuccino/include"
    "${CMAKE_SOURCE_DIR}/UB_Espresso/include"
    "${CMAKE_SOURCE_DIR}/UB_RHR/include"

    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}/mods/UB_ModVin/include"
)

target_link_libraries(UB_CLIENT PUBLIC
	glad::glad
	glm
	glfw
	imgui::imgui
	Vulkan::Vulkan
	
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

if(MSVC)
	target_compile_options(UB_CLIENT PRIVATE "/MP")
endif()

# Copy res folder
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/res"
	$<TARGET_FILE_DIR:UB_CLIENT>/res)
# Copy csfml
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/csfml"
	$<TARGET_FILE_DIR:UB_CLIENT>/csfml)
# Create mods folder for the mods to copy themselves into later
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory
	$<TARGET_FILE_DIR:UB_CLIENT>/mods)
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Cappuccino/include"
	$<TARGET_FILE_DIR:UB_CLIENT>/Cappuccino/include)
add_custom_command(
    TARGET UB_CLIENT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:UB_CLIENT>/Cappuccino/lib/)
add_custom_command(
    TARGET UB_CLIENT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:UB_CAPPUCCINO>
    $<TARGET_FILE_DIR:UB_CLIENT>/Cappuccino/lib/)

configure_file(config.h.in config.h)