cmake_minimum_required(VERSION 3.10)
project(UB_ESPRESSO)

set(UB_ESPRESSO_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/category.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/category.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/data.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/data.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/input_handler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/input_handler.cpp"
)

add_library(UB_ESPRESSO STATIC
	${UB_ESPRESSO_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/espresso FILES ${UB_ESPRESSO_SRC})
	target_compile_options(UB_ESPRESSO PRIVATE "/MP")
endif()

add_dependencies(UB_ESPRESSO
	UB_CAPPUCCINO
)

target_link_libraries(UB_ESPRESSO PUBLIC
	UB_CAPPUCCINO
	
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
	${TCC_LIBRARY}
)

target_include_directories(UB_ESPRESSO PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
	${CMAKE_INCLUDES}
)