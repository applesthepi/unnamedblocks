cmake_minimum_required(VERSION 3.15)
project(UB_CAPPUCCINO)

file(GLOB_RECURSE UB_CAPPUCCINO_SRC "include/*/*.cpp" "include/*.cpp")

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_library(UB_CAPPUCCINO STATIC ${UB_CAPPUCCINO_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/cappuccino FILES ${UB_CAPPUCCINO_SRC})
	target_compile_options(UB_CAPPUCCINO PRIVATE "/MP")
endif()

#include_directories(${LLVM_INCLUDE_DIRS})
#separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
#add_definitions(${LLVM_DEFINITIONS_LIST})

#llvm_map_components_to_libnames(llvm_libs support core irreader)

add_dependencies(
	UB_CAPPUCCINO ${UB_DEPENDENCIES} UB_ESPRESSO)

target_link_libraries(
	UB_CAPPUCCINO
	PUBLIC ${UB_DEPENDENCIES_LINK}
	UB_ESPRESSO
)

target_include_directories(
	UB_CAPPUCCINO
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
		   ${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})