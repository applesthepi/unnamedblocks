cmake_minimum_required(VERSION 3.15)
project(UB_RHR)

file (GLOB_RECURSE UB_RHR_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_RHR STATIC
	${UB_RHR_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/rhr FILES ${UB_RHR_SRC})
	target_compile_options(UB_RHR PRIVATE "/MP")
endif()

add_dependencies(UB_CLIENT
    UB_CAPPUCCINO
    UB_ESPRESSO
)

target_link_libraries(UB_RHR PUBLIC
	UB_CAPPUCCINO
	UB_ESPRESSO
	
	glad::glad
	glfw
	imgui
#	clip
	Vulkan::Vulkan
	freetype

#	imm32.lib
)

target_include_directories(UB_RHR PUBLIC SYSTEM
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"
	"${CMAKE_SOURCE_DIR}/vendor"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIR}
	${CMAKE_INCLUDES}

#	"${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/include/qt5"
)