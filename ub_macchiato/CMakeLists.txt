cmake_minimum_required(VERSION 3.15)
project(UB_MACCHIATO)

file(GLOB_RECURSE UB_MACCHIATO_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_MACCHIATO STATIC ${UB_MACCHIATO_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/MACCHIATO FILES ${UB_MACCHIATO_SRC})
	target_compile_options(UB_MACCHIATO PRIVATE "/MP")
endif()

add_dependencies(
	UB_MACCHIATO ${UB_DEPENDENCIES} UB_LATTE)

target_link_libraries(
	UB_MACCHIATO
	PUBLIC UB_LATTE)

target_include_directories(
	UB_MACCHIATO
	PUBLIC SYSTEM ${UB_INCLUDES})

target_link_libraries(
	UB_MACCHIATO
	PUBLIC ${UB_DEPENDENCIES_LINK}
)

target_include_directories(
	UB_MACCHIATO
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
	${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})