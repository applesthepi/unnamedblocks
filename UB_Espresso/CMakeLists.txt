cmake_minimum_required(VERSION 3.10)
project(UB_ESPRESSO)

set(UB_ESPRESSO_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/catagory/ModCatagory.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/catagory/ModCatagory.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/mod/ModData.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/mod/ModData.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/InputHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/InputHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Espresso.hpp"
)

# Preserve folders in visual studio structure
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso FILES ${UB_ESPRESSO_SRC})

add_library(UB_ESPRESSO STATIC
	${UB_ESPRESSO_SRC}
)

add_dependencies(UB_ESPRESSO
	UB_CAPPUCCINO
)

target_include_directories(UB_ESPRESSO PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_SOURCE_DIR}/UB_Cappuccino/include"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso"
)

target_link_libraries(UB_ESPRESSO PUBLIC
	UB_CAPPUCCINO
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
)

if(MSVC)
	target_compile_options(UB_ESPRESSO PRIVATE "/MP")
endif()

configure_file(config.h.in config.h)