cmake_minimum_required(VERSION 3.10)
project(UB_ESPRESSO)

set(UB_ESPRESSO_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/category.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/category.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/data.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/espresso/mod/data.cpp"
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
	)

target_include_directories(UB_ESPRESSO PUBLIC SYSTEM
	"${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
	)

target_link_libraries(UB_ESPRESSO PUBLIC
	${UB_DEPENDENCIES_LINK}
	#imm32.lib
	)

target_include_directories(UB_ESPRESSO PUBLIC SYSTEM
	${PROJECT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${UB_DEPENDENCIES_INCLUDE}
	${CMAKE_INCLUDES}
	)
