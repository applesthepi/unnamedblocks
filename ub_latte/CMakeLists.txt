cmake_minimum_required(VERSION 3.15)
project(UB_LATTE)

file(GLOB_RECURSE UB_LATTE_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_LATTE STATIC ${UB_LATTE_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/latte FILES ${UB_LATTE_SRC})
	target_compile_options(UB_LATTE PRIVATE "/MP")
endif()

add_dependencies(
	UB_LATTE ${UB_DEPENDENCIES})

target_include_directories(
	UB_LATTE
	PUBLIC SYSTEM ${UB_INCLUDES})

target_link_libraries(
	UB_LATTE
	PUBLIC ${UB_DEPENDENCIES_LINK}
)

target_include_directories(
	UB_LATTE
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
	${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})