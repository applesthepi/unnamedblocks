cmake_minimum_required(VERSION 3.15)
project(UB_MOD_VIN)

set(UB_MOD_VIN_SRC

	#======================================================================================================================
	#=========== Mod
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/Mod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/Mod.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/ModCategories.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/ModCategories.hpp"
	
	#======================================================================================================================
	#=========== System
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemMain.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemMain.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemPullEvents.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemPullEvents.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemCreateWindow.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemCreateWindow.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseX.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseX.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseY.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseY.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseLeft.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseLeft.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseMiddle.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseMiddle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseRight.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowMouseRight.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowPullScroll.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowPullScroll.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemSleep.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemSleep.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowVsync.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowVsync.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowFps.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowFps.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowRender.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/BlockSystemWindowRender.cpp"
	
	#======================================================================================================================
	#=========== Utility
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityMarkPoint.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityMarkPoint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGotoPoint.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGotoPoint.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGotoPointIf.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGotoPointIf.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityFunctionDefine.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityFunctionDefine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityFunctionCall.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityFunctionCall.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityThreadSummon.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityThreadSummon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityThreadKill.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityThreadKill.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalGetReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalGetReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalGetBool.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalGetBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalGetString.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalGetString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalSetReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalSetReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalSetBool.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalSetBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalSetString.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGlobalSetString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGetMicro.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityGetMicro.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListCreate.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListCreate.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListDestroy.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListDestroy.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListReserve.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListReserve.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListAdd.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListAdd.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListIndexReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListIndexReal.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListIndexBool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListIndexBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListIndexString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListIndexString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListInsert.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListInsert.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListRemove.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListRemove.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListReplace.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListReplace.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListSize.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/BlockUtilityListSize.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalReal.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalBool.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalBool.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalString.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityList.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityList.cpp"
	
	#======================================================================================================================
	#=========== Graphics
	#======================================================================================================================
	
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsTextureLoad.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsTextureLoad.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteCreate.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteCreate.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDestroy.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDestroy.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteTextureSet.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteTextureSet.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteEnable.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteEnable.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDisable.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDisable.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetPosition.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetPosition.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetRotation.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetRotation.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetScale.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetScale.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangePosition.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangePosition.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeRotation.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeRotation.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeScale.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeScale.hpp"
	
	#======================================================================================================================
	#=========== Booleans
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanForce.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanLog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanLog.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanSet.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanFlip.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanFlip.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanAnd.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanAnd.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanOr.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanOr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanNot.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/BlockBooleanNot.cpp"
	
	#======================================================================================================================
	#=========== Reals
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealForce.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealLog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealLog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealSet.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealPlus.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealPlus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealMinus.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealMinus.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealDivide.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealDivide.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealMultiply.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealMultiply.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealMod.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealMod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealFloor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealFloor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCeil.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCeil.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealRound.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealRound.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealFromBool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealFromBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealFromString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealFromString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealPi.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealPi.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealEulers.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealEulers.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealRandom.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealRandom.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealAbs.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealAbs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealPow.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealPow.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealLogarithmic.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealLogarithmic.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealSin.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealSin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCos.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCos.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealTan.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealTan.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompGr.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompGr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompLs.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompLs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompGrEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompGrEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompLsEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/BlockRealCompLsEq.cpp"
	
	#======================================================================================================================
	#=========== Strings
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringForce.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringForce.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringLog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringLog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringSubString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringSubString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringCharAt.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringCharAt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringCompEq.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringCompEq.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringLength.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringLength.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringSet.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFromReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFromReal.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFromRealInt.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFromRealInt.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFromBool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFromBool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFind.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringFind.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringCatFrom.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/BlockStringCatFrom.cpp"
)

add_library(UB_MOD_VIN MODULE
	${UB_MOD_VIN_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin FILES ${UB_MOD_VIN_SRC})
	target_compile_options(UB_MOD_VIN PRIVATE "/MP")
endif()

add_dependencies(UB_MOD_VIN
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

set_target_properties(UB_MOD_VIN PROPERTIES OUTPUT_NAME "mod_vin")

target_link_libraries(UB_MOD_VIN PUBLIC
	UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
	
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
)

target_include_directories(UB_MOD_VIN PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"
    "${CMAKE_SOURCE_DIR}/ub_rhr/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

add_custom_command(
	TARGET UB_MOD_VIN POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:UB_MOD_VIN>
	$<TARGET_FILE_DIR:UB_CLIENT>/mods/
)

configure_file(config.h.in config.h)