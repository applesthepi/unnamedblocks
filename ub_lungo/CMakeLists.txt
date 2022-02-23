cmake_minimum_required(VERSION 3.15)
project(UB_LUNGO)

file(GLOB_RECURSE UB_LUNGO_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_LUNGO STATIC ${UB_LUNGO_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/lungo FILES ${UB_LUNGO_SRC})
	target_compile_options(UB_LUNGO PRIVATE "/MP")
endif()

add_dependencies(
	UB_LUNGO ${UB_DEPENDENCIES} UB_LATTE)

target_link_libraries(
	UB_LUNGO
	PUBLIC UB_LATTE)

target_include_directories(
	UB_LUNGO
	PUBLIC SYSTEM ${UB_INCLUDES})

target_link_libraries(
	UB_LUNGO
	PUBLIC ${UB_DEPENDENCIES_LINK}
)

target_include_directories(
	UB_LUNGO
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
	${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})