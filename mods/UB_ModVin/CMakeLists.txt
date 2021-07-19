cmake_minimum_required(VERSION 3.15)
project(UB_MOD_VIN)

set(UB_MOD_VIN_SRC

	#======================================================================================================================
	#=========== Mod
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/Mod.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/ModCategories.hpp"
	
	#======================================================================================================================
	#=========== System
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemMain.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemMain.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemPullEvents.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemPullEvents.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemCreateWindow.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemCreateWindow.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseX.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseX.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseY.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseY.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseLeft.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseLeft.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseMiddle.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseMiddle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseRight.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowMouseRight.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowPullScroll.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowPullScroll.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemSleep.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemSleep.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowVsync.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowVsync.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowFps.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowFps.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowRender.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/system/BlockSystemWindowRender.cpp"
	
	#======================================================================================================================
	#=========== Utility
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityMarkPoint.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityMarkPoint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPoint.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPoint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPointIf.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGotoPointIf.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionDefine.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionDefine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionCall.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityFunctionCall.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadSummon.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadSummon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadKill.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityThreadKill.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetBool.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetString.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalGetString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetBool.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetString.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGlobalSetString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGetMicro.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityGetMicro.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListCreate.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListCreate.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListDestroy.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListDestroy.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListReserve.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListReserve.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListAdd.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListAdd.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListIndexReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListIndexReal.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListIndexBool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListIndexBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListIndexString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListIndexString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListInsert.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListInsert.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListRemove.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListRemove.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListReplace.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListReplace.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListSize.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/BlockUtilityListSize.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalBool.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalString.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityGlobalString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityList.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/utility/aid/UtilityList.cpp"
	
	#======================================================================================================================
	#=========== Graphics
	#======================================================================================================================
	
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsTextureLoad.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsTextureLoad.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteCreate.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteCreate.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDestroy.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDestroy.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteTextureSet.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteTextureSet.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteEnable.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteEnable.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDisable.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteDisable.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetPosition.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetPosition.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetRotation.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetRotation.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetScale.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteSetScale.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangePosition.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangePosition.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeRotation.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeRotation.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeScale.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/graphics/BlockGraphicsSpriteChangeScale.hpp"
	
	#======================================================================================================================
	#=========== Booleans
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanForce.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanLog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanLog.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanSet.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanFlip.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanFlip.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanAnd.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanAnd.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanOr.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanOr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanNot.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/booleans/BlockBooleanNot.cpp"
	
	#======================================================================================================================
	#=========== Reals
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealForce.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSet.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPlus.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPlus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMinus.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMinus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealDivide.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealDivide.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMultiply.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMultiply.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMod.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealMod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFloor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFloor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCeil.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCeil.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRound.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRound.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromBool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealFromString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPi.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPi.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealEulers.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealEulers.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRandom.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealRandom.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealAbs.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealAbs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPow.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealPow.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLogarithmic.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealLogarithmic.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSin.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealSin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCos.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCos.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealTan.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealTan.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGr.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLs.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGrEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompGrEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLsEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/reals/BlockRealCompLsEq.cpp"
	
	#======================================================================================================================
	#=========== Strings
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringForce.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSubString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSubString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCharAt.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCharAt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCompEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCompEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLength.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringLength.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSet.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromReal.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromRealInt.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromRealInt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromBool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFromBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFind.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringFind.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCatFrom.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin/blocks/strings/BlockStringCatFrom.cpp"
)

add_library(UB_MOD_VIN MODULE
	${UB_MOD_VIN_SRC}
)

add_dependencies(UB_CLIENT
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

# Preserve folders in visual studio structure
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/ModVin FILES ${UB_MOD_VIN_SRC})

set_target_properties(UB_MOD_VIN PROPERTIES OUTPUT_NAME "ModVin")

target_include_directories(UB_MOD_VIN PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_SOURCE_DIR}/UB_RHR"
	"${CMAKE_SOURCE_DIR}/UB_RHR/include"
	"${CMAKE_SOURCE_DIR}/UB_Espresso/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	# "${csfml_include}"
)

target_link_libraries(UB_MOD_VIN PUBLIC
	UB_RHR
	UB_ESPRESSO
	UB_CAPPUCCINO
)

add_custom_command(
	TARGET UB_MOD_VIN POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:UB_MOD_VIN>
	$<TARGET_FILE_DIR:UB_CLIENT>/mods/
)

configure_file(config.h.in config.h)