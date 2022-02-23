cmake_minimum_required(VERSION 3.10)
project(UB_ESPRESSO)

file(GLOB_RECURSE UB_ESPRESSO_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_ESPRESSO STATIC ${UB_ESPRESSO_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/espresso FILES ${UB_ESPRESSO_SRC})
	target_compile_options(UB_ESPRESSO PRIVATE "/MP")
endif()

add_dependencies(
	UB_ESPRESSO ${UB_DEPENDENCIES} UB_LATTE)

target_link_libraries(
	UB_ESPRESSO
	PUBLIC UB_LATTE)

target_include_directories(
	UB_ESPRESSO
	PUBLIC SYSTEM ${UB_INCLUDES})

target_link_libraries(
	UB_ESPRESSO
	PUBLIC ${UB_DEPENDENCIES_LINK}
)

target_include_directories(
	UB_ESPRESSO
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
		   ${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})
