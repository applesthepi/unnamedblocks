cmake_minimum_required(VERSION 3.10)

project(ModVin)
configure_file(config.h.in config.h)
set(VERSION_MOD_VIN "0.0.0" CACHE STRING "Version of Mod Vin")

set(ModVin_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockTest.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockMain.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockMain.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockTestRender.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockTestRender.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockTestEvent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockTestEvent.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockYes.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/BlockYes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.h"
)

add_library(ModVin MODULE
	${ModVin_SRC}
	)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${ModVin_SRC})
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${ModVin_SRC})

set_target_properties(ModVin PROPERTIES OUTPUT_NAME "ModVin")

target_include_directories(ModVin PUBLIC
	"${PROJECT_BINARY_DIR}"
	"RHR"
	"RHR/include"
	"Espresso/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${csfml_include}"
)

target_link_libraries(ModVin PUBLIC
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
	TARGET ModVin POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:ModVin>
	$<TARGET_FILE_DIR:unnamedblocks>/mods/)