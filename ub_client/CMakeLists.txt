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
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT UB_CLIENT)
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
	imgui
#	clip
	Vulkan::Vulkan
	freetype

	imm32.lib
)

target_include_directories(UB_CLIENT PUBLIC SYSTEM
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"
    "${CMAKE_SOURCE_DIR}/ub_rhr/include"
	"${CMAKE_SOURCE_DIR}/vendor"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIR}
	${CMAKE_INCLUDES}

#	"${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/include/qt5"
)

# Copy Resources
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/res"
	$<TARGET_FILE_DIR:UB_CLIENT>/res)

# Mods Folder
add_custom_command(
	TARGET UB_CLIENT POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory
	$<TARGET_FILE_DIR:UB_CLIENT>/mods)

#add_custom_command(
#	TARGET UB_CLIENT POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	"${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
#	$<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/include)
#add_custom_command(
#    TARGET UB_CLIENT POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E make_directory
#    $<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/lib/)
#add_custom_command(
#    TARGET UB_CLIENT POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy
#    $<TARGET_FILE:UB_CAPPUCCINO_TCC>
#    $<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/lib/)
#add_custom_command(
#	TARGET UB_CLIENT POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy
#	"${CMAKE_SOURCE_DIR}/cmake/bin/libtcc.dll"
#	$<TARGET_FILE_DIR:UB_CLIENT>/)
#add_custom_command(
#		TARGET UB_CLIENT POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy_directory
#		"${CMAKE_SOURCE_DIR}/tcc/include"
#		$<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/include)
#add_custom_command(
#		TARGET UB_CLIENT POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy
#		"${CMAKE_SOURCE_DIR}/build/config.h"
#		$<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/include/)
#add_custom_command(
#		TARGET UB_CLIENT POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy
#		"${CMAKE_SOURCE_DIR}/res/cap_config.h"
#		$<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/include/)
#add_custom_command(
#		TARGET UB_CLIENT POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy
#		"${CMAKE_SOURCE_DIR}/tcc/lib/libtcc1-64.a"
#		$<TARGET_FILE_DIR:UB_CLIENT>/ub_cappuccino/lib/)
	
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)