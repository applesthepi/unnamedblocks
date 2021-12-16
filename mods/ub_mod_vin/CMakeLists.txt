cmake_minimum_required(VERSION 3.15)
project(UB_MOD_VIN)

file (GLOB_RECURSE UB_MOD_VIN_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_MOD_VIN MODULE
	${UB_MOD_VIN_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin FILES ${UB_MOD_VIN_SRC})
	target_compile_options(UB_MOD_VIN PRIVATE "/MP")
endif()

add_dependencies(UB_MOD_VIN
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

set_target_properties(UB_MOD_VIN PROPERTIES OUTPUT_NAME "mod_vin")

target_link_libraries(UB_MOD_VIN PUBLIC
	UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
	
	glad::glad
	glfw
#	imgui::imgui
	Vulkan::Vulkan
	freetype
)

target_include_directories(UB_MOD_VIN PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"
    "${CMAKE_SOURCE_DIR}/ub_rhr/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

add_custom_command(
	TARGET UB_MOD_VIN POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:UB_MOD_VIN>
	$<TARGET_FILE_DIR:UB_CLIENT>/mods/
)