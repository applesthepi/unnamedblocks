cmake_minimum_required(VERSION 3.15)
project(UB_MOCHA)

file(GLOB_RECURSE UB_MOCHA_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_MOCHA STATIC ${UB_MOCHA_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/mocha FILES ${UB_MOCHA_SRC})
	target_compile_options(UB_MOCHA PRIVATE "/MP")
endif()

add_dependencies(
	UB_MOCHA ${UB_DEPENDENCIES} UB_CAPPUCCINO
	# UB_LUNGO
)

target_link_libraries(
	UB_MOCHA
	PUBLIC UB_CAPPUCCINO
	# UB_LUNGO
)

target_include_directories(
	UB_MOCHA
	PUBLIC SYSTEM ${UB_INCLUDES})

target_link_libraries(
	UB_MOCHA
	PUBLIC ${UB_DEPENDENCIES_LINK}
)

target_include_directories(
	UB_MOCHA
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
	${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})