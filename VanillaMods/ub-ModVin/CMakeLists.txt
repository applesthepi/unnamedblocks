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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.h"
	
	#======================================================================================================================
	#=========== System
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemMain.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemMain.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemPullEvents.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemPullEvents.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemCreateWindow.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemCreateWindow.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseX.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseX.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseY.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseY.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseLeft.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseLeft.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseMiddle.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseMiddle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseRight.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseRight.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowPullScroll.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowPullScroll.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemSleep.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemSleep.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowVsync.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowVsync.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowFps.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowFps.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowRender.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowRender.cpp"
	
	#======================================================================================================================
	#=========== Utility
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityMarkPoint.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityMarkPoint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPoint.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPoint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPointIf.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPointIf.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionDefine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionDefine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionCall.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionCall.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadSummon.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadSummon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadKill.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadKill.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetReal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetBool.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetString.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetReal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetBool.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetString.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetString.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalReal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalBool.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalString.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalString.cpp"
	
	#======================================================================================================================
	#=========== Graphics
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsTextureLoad.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsTextureLoad.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteCreate.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteCreate.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDestroy.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDestroy.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteTextureSet.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteTextureSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteEnable.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteEnable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDisable.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDisable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetPosition.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetPosition.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetRotation.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetRotation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetScale.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetScale.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangePosition.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangePosition.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeRotation.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeRotation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeScale.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeScale.cpp"
	
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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLogarithmic.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLogarithmic.cpp"
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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSubString.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSubString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCharAt.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCharAt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCompEq.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCompEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLength.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLength.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSet.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromReal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromReal.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromRealInt.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromRealInt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromBool.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFind.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFind.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCatFrom.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCatFrom.cpp"
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
	sfml-network
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
