cmake_minimum_required(VERSION 3.15)
project(UB_CAPPUCCINO)

file(GLOB_RECURSE UB_CAPPUCCINO_SRC "include/*/*.cpp" "include/*.cpp")

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_library(UB_CAPPUCCINO STATIC ${UB_CAPPUCCINO_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino FILES ${UB_CAPPUCCINO_SRC})
	target_compile_options(UB_CAPPUCCINO PRIVATE "/MP")
endif()

add_dependencies(
	UB_CAPPUCCINO ${UB_DEPENDENCIES})

target_link_libraries(
	UB_CAPPUCCINO
	PUBLIC ${UB_DEPENDENCIES_LINK}
	# imm32.lib
)

target_include_directories(
	UB_CAPPUCCINO
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
		   ${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})

# TODO: what is this for?
set_target_properties(UB_CAPPUCCINO PROPERTIES PREFIX "lib")
