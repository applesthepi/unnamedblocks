cmake_minimum_required(VERSION 3.10)

project(ModVin)
configure_file(config.h.in config.h)
set(VERSION_MOD_VIN "0.0.0" CACHE STRING "Version of Mod Vin")

set(ModVin_SRC

	#======================================================================================================================
	#=========== Mod
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.h"
	
	#======================================================================================================================
	#=========== System
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemMain.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemMain.cpp"
	
	#======================================================================================================================
	#=========== Objects
	#======================================================================================================================
	
	#======================================================================================================================
	#=========== Booleans
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanForce.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanLog.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanLog.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanSet.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanFlip.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanFlip.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanAnd.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanAnd.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanOr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanOr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanNot.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanNot.cpp"
	
	#======================================================================================================================
	#=========== Reals
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealForce.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLog.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLog.cpp"
	
	#======================================================================================================================
	#=========== Strings
	#======================================================================================================================

	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringForce.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLog.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLog.cpp"
)

add_library(ModVin MODULE
	${ModVin_SRC}
	)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${ModVin_SRC})
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${ModVin_SRC})

set_target_properties(ModVin PROPERTIES OUTPUT_NAME "ModVin")

target_include_directories(ModVin PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_SOURCE_DIR}/RHR"
	"${CMAKE_SOURCE_DIR}RHR/include"
	"${CMAKE_SOURCE_DIR}Espresso/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${csfml_include}"
)

target_link_libraries(ModVin PUBLIC
	sfml-graphics
	sfml-system
	RHR
	Espresso
	Cappuccino
	optimized "${csfml-audio}"
    optimized "${csfml-network}"
    optimized "${csfml-graphics}"
    optimized "${csfml-system}"
    optimized "${csfml-window}"
	debug "${csfml-audio-d}"
    debug "${csfml-network-d}"
    debug "${csfml-graphics-d}"
    debug "${csfml-system-d}"
    debug "${csfml-window-d}"
)

add_custom_command(
	TARGET ModVin POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:ModVin>
	$<TARGET_FILE_DIR:unnamedblocks>/mods/)
