cmake_minimum_required(VERSION 3.15)
project(UB_RHR)

file(GLOB_RECURSE UB_RHR_SRC "include/*/*.cpp" "include/*.cpp")

add_library(UB_RHR STATIC ${UB_RHR_SRC})

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/rhr FILES ${UB_RHR_SRC})
	target_compile_options(UB_RHR PRIVATE "/MP")
endif()

add_dependencies(
	UB_RHR ${UB_DEPENDENCIES} UB_CAPPUCCINO UB_ESPRESSO)

target_link_libraries(
	UB_RHR
	PUBLIC UB_CAPPUCCINO UB_ESPRESSO)

target_include_directories(
	UB_RHR
	PUBLIC SYSTEM "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
		   "${CMAKE_SOURCE_DIR}/ub_espresso/include")

target_link_libraries(
	UB_RHR
	PUBLIC ${UB_DEPENDENCIES_LINK}
	# imm32.lib
)

target_include_directories(
	UB_RHR
	PUBLIC SYSTEM ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
		   ${UB_DEPENDENCIES_INCLUDE} ${CMAKE_INCLUDES})
# ##################################################################################################
# ##################################################################################################
# ##################################################################################################
# ##################################################################################################
# TODO: ADDDDDDDDDDD EEEEEEVVVVVVEEEERRRRRYYYYY WHERE TODO: ??? if(LINUX) else()
# target_link_libraries(UB_RHR PUBLIC imm32.lib ) endif()
