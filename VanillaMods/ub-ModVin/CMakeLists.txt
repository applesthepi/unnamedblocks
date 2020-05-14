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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSet.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPlus.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPlus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMinus.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMinus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealDivide.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealDivide.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMultiply.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMultiply.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFloor.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFloor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCeil.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCeil.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRound.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRound.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromBool.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromString.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPi.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPi.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealEulers.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealEulers.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRandom.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRandom.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealAbs.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealAbs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPow.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPow.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLog.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSin.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCos.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCos.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealTan.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealTan.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompEq.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLs.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGrEq.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGrEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLsEq.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLsEq.cpp"
	
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
