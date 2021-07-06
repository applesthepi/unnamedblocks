#include "Mod.hpp"

// system

#include "blocks/system/BlockSystemMain.hpp"
//#include "blocks/system/BlockSystemPullEvents.h"
//#include "blocks/system/BlockSystemCreateWindow.h"
//#include "blocks/system/BlockSystemWindowMouseX.h"
//#include "blocks/system/BlockSystemWindowMouseY.h"
//#include "blocks/system/BlockSystemWindowMouseLeft.h"
//#include "blocks/system/BlockSystemWindowMouseMiddle.h"
//#include "blocks/system/BlockSystemWindowMouseRight.h"
//#include "blocks/system/BlockSystemWindowPullScroll.h"
//#include "blocks/system/BlockSystemSleep.h"
//#include "blocks/system/BlockSystemWindowVsync.h"
//#include "blocks/system/BlockSystemWindowFps.h"
//#include "blocks/system/BlockSystemWindowRender.h"
//
//// utility
//
//#include "blocks/utility/BlockUtilityMarkPoint.h"
//#include "blocks/utility/BlockUtilityGotoPoint.h"
//#include "blocks/utility/BlockUtilityGotoPointIf.h"
//#include "blocks/utility/BlockUtilityFunctionDefine.h"
//#include "blocks/utility/BlockUtilityFunctionCall.h"
//#include "blocks/utility/BlockUtilityThreadSummon.h"
//#include "blocks/utility/BlockUtilityThreadKill.h"
//#include "blocks/utility/BlockUtilityGetMicro.h"
//#include "blocks/utility/BlockUtilityGlobalSetReal.h"
//#include "blocks/utility/BlockUtilityGlobalGetReal.h"
//#include "blocks/utility/BlockUtilityListCreate.h"
//#include "blocks/utility/BlockUtilityListDestroy.h"
//#include "blocks/utility/BlockUtilityListIndexReal.h"
//#include "blocks/utility/BlockUtilityListIndexBool.h"
//#include "blocks/utility/BlockUtilityListIndexString.h"
//#include "blocks/utility/BlockUtilityListInsert.h"
//#include "blocks/utility/BlockUtilityListRemove.h"
//#include "blocks/utility/BlockUtilityListReplace.h"
//#include "blocks/utility/BlockUtilityListReserve.h"
//#include "blocks/utility/BlockUtilityListAdd.h"
//#include "blocks/utility/BlockUtilityListSize.h"
//
//// graphics
//
//#include "blocks/graphics/BlockGraphicsTextureLoad.h"
//#include "blocks/graphics/BlockGraphicsSpriteCreate.h"
//#include "blocks/graphics/BlockGraphicsSpriteDestroy.h"
//#include "blocks/graphics/BlockGraphicsSpriteTextureSet.h"
//#include "blocks/graphics/BlockGraphicsSpriteEnable.h"
//#include "blocks/graphics/BlockGraphicsSpriteDisable.h"
//#include "blocks/graphics/BlockGraphicsSpriteSetPosition.h"
//#include "blocks/graphics/BlockGraphicsSpriteSetRotation.h"
//#include "blocks/graphics/BlockGraphicsSpriteSetScale.h"
//#include "blocks/graphics/BlockGraphicsSpriteChangePosition.h"
//#include "blocks/graphics/BlockGraphicsSpriteChangeRotation.h"
//#include "blocks/graphics/BlockGraphicsSpriteChangeScale.h"
//
//// booleans
//
//#include "blocks/booleans/BlockBooleanForce.h"
//#include "blocks/booleans/BlockBooleanLog.h"
//#include "blocks/booleans/BlockBooleanSet.h"
//#include "blocks/booleans/BlockBooleanFlip.h"
//#include "blocks/booleans/BlockBooleanAnd.h"
//#include "blocks/booleans/BlockBooleanOr.h"
//#include "blocks/booleans/BlockBooleanNot.h"
//
//// reals
//
//#include "blocks/reals/BlockRealForce.h"
//#include "blocks/reals/BlockRealLog.h"
//#include "blocks/reals/BlockRealSet.h"
//#include "blocks/reals/BlockRealPlus.h"
//#include "blocks/reals/BlockRealMinus.h"
//#include "blocks/reals/BlockRealDivide.h"
//#include "blocks/reals/BlockRealMultiply.h"
//#include "blocks/reals/BlockRealMod.h"
//#include "blocks/reals/BlockRealFloor.h"
//#include "blocks/reals/BlockRealCeil.h"
//#include "blocks/reals/BlockRealRound.h"
//#include "blocks/reals/BlockRealFromBool.h"
//#include "blocks/reals/BlockRealFromString.h"
//#include "blocks/reals/BlockRealPi.h"
//#include "blocks/reals/BlockRealEulers.h"
//#include "blocks/reals/BlockRealRandom.h"
//#include "blocks/reals/BlockRealLogarithmic.h"
//#include "blocks/reals/BlockRealAbs.h"
//#include "blocks/reals/BlockRealPow.h"
//#include "blocks/reals/BlockRealLog.h"
//#include "blocks/reals/BlockRealSin.h"
//#include "blocks/reals/BlockRealCos.h"
//#include "blocks/reals/BlockRealTan.h"
//#include "blocks/reals/BlockRealCompEq.h"
//#include "blocks/reals/BlockRealCompGr.h"
//#include "blocks/reals/BlockRealCompLs.h"
//#include "blocks/reals/BlockRealCompGrEq.h"
//#include "blocks/reals/BlockRealCompLsEq.h"
//
//// strings
//
//#include "blocks/strings/BlockStringForce.h"
//#include "blocks/strings/BlockStringLog.h"
//#include "blocks/strings/BlockStringSubString.h"
//#include "blocks/strings/BlockStringCharAt.h"
//#include "blocks/strings/BlockStringCompEq.h"
//#include "blocks/strings/BlockStringLength.h"
//#include "blocks/strings/BlockStringSet.h"
//#include "blocks/strings/BlockStringFromReal.h"
//#include "blocks/strings/BlockStringFromRealInt.h"
//#include "blocks/strings/BlockStringFromBool.h"
//#include "blocks/strings/BlockStringFind.h"
//#include "blocks/strings/BlockStringCatFrom.h"

UB_EXPORT void Initialization(ModData* data)
{
	// ============================================
	// ==== Mod Information
	// ============================================

	data->ModUnlocalizedName = "ModVin";
	data->ModDisplayName = "Vanilla Mod";
	data->Version = ModVinVersion;

	// ============================================
	// ==== Categories
	// ============================================

	data->RegisterCategory(new CategorySystem(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryUtility(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryGraphics(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryReals(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryStrings(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryBooleans(data->ModUnlocalizedName, data->ModDisplayName));

	// ============================================
	// ==== Blocks
	// ============================================

	// system

	data->RegisterBlock(new BlockSystemMain());
	//data->RegisterBlock(new BlockSystemSleep());
	//data->RegisterBlock(new BlockSystemCreateWindow());
	//data->RegisterBlock(new BlockSystemPullEvents());
	//data->RegisterBlock(new BlockSystemWindowRender());
	//data->RegisterBlock(new BlockSystemWindowVsync());
	//data->RegisterBlock(new BlockSystemWindowFps());
	//data->RegisterBlock(new BlockSystemWindowMouseX());
	//data->RegisterBlock(new BlockSystemWindowMouseY());
	//data->RegisterBlock(new BlockSystemWindowMouseLeft());
	//data->RegisterBlock(new BlockSystemWindowMouseMiddle());
	//data->RegisterBlock(new BlockSystemWindowMouseRight());
	//data->RegisterBlock(new BlockSystemWindowPullScroll());

	//// utility

	//data->RegisterBlock(new BlockUtilityMarkPoint());
	//data->RegisterBlock(new BlockUtilityGotoPoint());
	//data->RegisterBlock(new BlockUtilityGotoPointIf());
	//data->RegisterBlock(new BlockUtilityFunctionDefine());
	//data->RegisterBlock(new BlockUtilityFunctionCall());
	//data->RegisterBlock(new BlockUtilityThreadSummon());
	//data->RegisterBlock(new BlockUtilityThreadKill());
	//data->RegisterBlock(new BlockUtilityGetMicro());
	//data->RegisterBlock(new BlockUtilityGlobalSetReal());
	//data->RegisterBlock(new BlockUtilityGlobalGetReal());
	//data->RegisterBlock(new BlockUtilityListCreate());
	//data->RegisterBlock(new BlockUtilityListDestroy());
	//data->RegisterBlock(new BlockUtilityListIndexReal());
	//data->RegisterBlock(new BlockUtilityListIndexBool());
	//data->RegisterBlock(new BlockUtilityListIndexString());
	//data->RegisterBlock(new BlockUtilityListInsert());
	//data->RegisterBlock(new BlockUtilityListRemove());
	//data->RegisterBlock(new BlockUtilityListReplace());
	//data->RegisterBlock(new BlockUtilityListReserve());
	//data->RegisterBlock(new BlockUtilityListAdd());
	//data->RegisterBlock(new BlockUtilityListSize());


	//// graphics

	//data->RegisterBlock(new BlockGraphicsTextureLoad());
	//data->RegisterBlock(new BlockGraphicsSpriteCreate());
	//data->RegisterBlock(new BlockGraphicsSpriteDestroy());
	//data->RegisterBlock(new BlockGraphicsSpriteTextureSet());
	//data->RegisterBlock(new BlockGraphicsSpriteEnable());
	//data->RegisterBlock(new BlockGraphicsSpriteDisable());
	//data->RegisterBlock(new BlockGraphicsSpriteSetPosition());
	//data->RegisterBlock(new BlockGraphicsSpriteSetRotation());
	//data->RegisterBlock(new BlockGraphicsSpriteSetScale());
	//data->RegisterBlock(new BlockGraphicsSpriteChangePosition());
	//data->RegisterBlock(new BlockGraphicsSpriteChangeRotation());
	//data->RegisterBlock(new BlockGraphicsSpriteChangeScale());

	//// booleans

	//data->RegisterBlock(new BlockBooleanForce());
	//data->RegisterBlock(new BlockBooleanLog());

	//data->RegisterBlock(new BlockBooleanSet());
	//data->RegisterBlock(new BlockBooleanNot());
	//data->RegisterBlock(new BlockBooleanFlip());
	//data->RegisterBlock(new BlockBooleanAnd());
	//data->RegisterBlock(new BlockBooleanOr());

	//// reals

	//data->RegisterBlock(new BlockRealForce());
	//data->RegisterBlock(new BlockRealLog());
	//data->RegisterBlock(new BlockRealSet());
	//data->RegisterBlock(new BlockRealFromBool());
	//data->RegisterBlock(new BlockRealFromString());
	//data->RegisterBlock(new BlockRealPi());
	//data->RegisterBlock(new BlockRealEulers());
	//data->RegisterBlock(new BlockRealPlus());
	//data->RegisterBlock(new BlockRealMinus());
	//data->RegisterBlock(new BlockRealDivide());
	//data->RegisterBlock(new BlockRealMultiply());
	//data->RegisterBlock(new BlockRealMod());
	//data->RegisterBlock(new BlockRealRandom());
	//data->RegisterBlock(new BlockRealPow());
	//data->RegisterBlock(new BlockRealLogarithmic());
	//data->RegisterBlock(new BlockRealFloor());
	//data->RegisterBlock(new BlockRealCeil());
	//data->RegisterBlock(new BlockRealRound());
	//data->RegisterBlock(new BlockRealAbs());
	//data->RegisterBlock(new BlockRealLog());
	//data->RegisterBlock(new BlockRealSin());
	//data->RegisterBlock(new BlockRealCos());
	//data->RegisterBlock(new BlockRealTan());
	//data->RegisterBlock(new BlockRealCompEq());
	//data->RegisterBlock(new BlockRealCompGr());
	//data->RegisterBlock(new BlockRealCompLs());
	//data->RegisterBlock(new BlockRealCompGrEq());
	//data->RegisterBlock(new BlockRealCompLsEq());

	//// strings

	//data->RegisterBlock(new BlockStringForce());
	//data->RegisterBlock(new BlockStringLog());
	//data->RegisterBlock(new BlockStringSet());
	//data->RegisterBlock(new BlockStringCatFrom());
	//data->RegisterBlock(new BlockStringFromReal());
	//data->RegisterBlock(new BlockStringFromRealInt());
	//data->RegisterBlock(new BlockStringFromBool());
	//data->RegisterBlock(new BlockStringLength());
	//data->RegisterBlock(new BlockStringCharAt());
	//data->RegisterBlock(new BlockStringSubString());
	//data->RegisterBlock(new BlockStringFind());
	//data->RegisterBlock(new BlockStringCompEq());
}
