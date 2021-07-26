cmake_minimum_required(VERSION 3.15)
project(UB_CLIENT)

set(UB_CLIENT_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/mod_loader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/mod_loader.hpp"
)

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_executable(UB_CLIENT
	${UB_CLIENT_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${UB_CLIENT_SRC})
	target_compile_options(UB_CLIENT PRIVATE "/MP")
	set_target_properties(UB_CLIENT PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:UB_CLIENT>)
endif()

add_dependencies(UB_CLIENT
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

target_link_libraries(UB_CLIENT PUBLIC
	UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
	
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
)

target_include_directories(UB_CLIENT PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"
    "${CMAKE_SOURCE_DIR}/ub_rhr/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

# Copy res folder
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/res"
	$<TARGET_FILE_DIR:UB_CLIENT>/res)
# Copy csfml
#add_custom_command(
#	TARGET UB_CLIENT POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	"${CMAKE_SOURCE_DIR}/csfml"
#	$<TARGET_FILE_DIR:UB_CLIENT>/csfml)
# Create mods folder for the mods to copy themselves into later
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory
	$<TARGET_FILE_DIR:UB_CLIENT>/mods)
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
	$<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/include)
add_custom_command(
    TARGET UB_CLIENT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/lib/)
add_custom_command(
    TARGET UB_CLIENT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:UB_CAPPUCCINO>
    $<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/lib/)
	
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)