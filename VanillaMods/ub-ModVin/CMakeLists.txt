cmake_minimum_required(VERSION 3.10)

project(ModVin)
configure_file(config.h.in config.h)
set(VERSION_MOD_VIN "0.0" CACHE STRING "Version of Mod Vin")

set(ModVin_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockPrint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockPrint.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.h"
)

add_library(ModVin-Dynamic MODULE
	${ModVin_SRC}
	)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${ModVin_SRC})

add_library(ModVin-Static STATIC
	${ModVin_SRC}
	)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${ModVin_SRC})

set_target_properties(ModVin-Dynamic PROPERTIES OUTPUT_NAME "ModVin")

target_include_directories(ModVin-Dynamic PUBLIC
	"${PROJECT_BINARY_DIR}"
	"RHR"
	"RHR/include"
	"Espresso/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${csfml_include}"
)

target_link_libraries(ModVin-Dynamic PUBLIC
	sfml-graphics
	sfml-system
	RHR
	Espresso
	"${csfml-audio}"
        "${csfml-network}"
        "${csfml-graphics}"
        "${csfml-system}"
        "${csfml-window}"
)

add_custom_command(
	TARGET ModVin-Dynamic POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:ModVin-Dynamic>
	$<TARGET_FILE_DIR:unnamedblocks>/mods/)

#=========================================================================================================
#============================[ STATIC ]===================================================================
#=========================================================================================================

add_library(ModVin-Static STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockPrint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockPrint.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.h"
	)

set_target_properties(ModVin-Static PROPERTIES OUTPUT_NAME "ModVin-s")
set_target_properties(ModVin-Static PROPERTIES PREFIX "lib")
set_target_properties(ModVin-Static PROPERTIES SUFFIX ".a")

target_include_directories(ModVin-Static PUBLIC
	"${PROJECT_BINARY_DIR}"
	"RHR"
	"RHR/include"
	"Espresso/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${csfml_include}"
)

target_link_libraries(ModVin-Static PUBLIC
	sfml-graphics
	sfml-system
	RHR
	Espresso
	"${csfml-audio}"
        "${csfml-network}"
        "${csfml-graphics}"
        "${csfml-system}"
        "${csfml-window}"
)

add_custom_command(
	TARGET ModVin-Static POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:ModVin-Static>
	$<TARGET_FILE_DIR:unnamedblocks>/mods/)
