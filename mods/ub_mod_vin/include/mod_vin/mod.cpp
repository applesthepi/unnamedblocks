#include "mod.hpp"

// system

#include "blocks/system/main.hpp"
/*#include "blocks/system/BlockSystemPullEvents.hpp"
#include "blocks/system/BlockSystemCreateWindow.hpp"
#include "blocks/system/BlockSystemWindowMouseX.hpp"
#include "blocks/system/BlockSystemWindowMouseY.hpp"
#include "blocks/system/BlockSystemWindowMouseLeft.hpp"
#include "blocks/system/BlockSystemWindowMouseMiddle.hpp"
#include "blocks/system/BlockSystemWindowMouseRight.hpp"
#include "blocks/system/BlockSystemWindowPullScroll.hpp"
#include "blocks/system/BlockSystemSleep.hpp"
#include "blocks/system/BlockSystemWindowVsync.hpp"
#include "blocks/system/BlockSystemWindowFps.hpp"
#include "blocks/system/BlockSystemWindowRender.hpp"

// utility

#include "blocks/utility/BlockUtilityMarkPoint.hpp"
#include "blocks/utility/BlockUtilityGotoPoint.hpp"
#include "blocks/utility/BlockUtilityGotoPointIf.hpp"
#include "blocks/utility/BlockUtilityFunctionDefine.hpp"
#include "blocks/utility/BlockUtilityFunctionCall.hpp"
#include "blocks/utility/BlockUtilityThreadSummon.hpp"
#include "blocks/utility/BlockUtilityThreadKill.hpp"
#include "blocks/utility/BlockUtilityGetMicro.hpp"
#include "blocks/utility/BlockUtilityGlobalSetReal.hpp"
#include "blocks/utility/BlockUtilityGlobalGetReal.hpp"
#include "blocks/utility/BlockUtilityListCreate.hpp"
#include "blocks/utility/BlockUtilityListDestroy.hpp"
#include "blocks/utility/BlockUtilityListIndexReal.hpp"
#include "blocks/utility/BlockUtilityListIndexBool.hpp"
#include "blocks/utility/BlockUtilityListIndexString.hpp"
#include "blocks/utility/BlockUtilityListInsert.hpp"
#include "blocks/utility/BlockUtilityListRemove.hpp"
#include "blocks/utility/BlockUtilityListReplace.hpp"
#include "blocks/utility/BlockUtilityListReserve.hpp"
#include "blocks/utility/BlockUtilityListAdd.hpp"
#include "blocks/utility/BlockUtilityListSize.hpp"

// graphics

//#include "blocks/graphics/BlockGraphicsTextureLoad.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteCreate.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteDestroy.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteTextureSet.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteEnable.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteDisable.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteSetPosition.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteSetRotation.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteSetScale.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteChangePosition.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteChangeRotation.hpp"
//#include "blocks/graphics/BlockGraphicsSpriteChangeScale.hpp"

// booleans

#include "blocks/booleans/BlockBooleanForce.hpp"
#include "blocks/booleans/BlockBooleanLog.hpp"
#include "blocks/booleans/BlockBooleanSet.hpp"
#include "blocks/booleans/BlockBooleanFlip.hpp"
#include "blocks/booleans/BlockBooleanAnd.hpp"
#include "blocks/booleans/BlockBooleanOr.hpp"
#include "blocks/booleans/BlockBooleanNot.hpp"

// reals

#include "blocks/reals/BlockRealForce.hpp"
#include "blocks/reals/BlockRealLog.hpp"
#include "blocks/reals/BlockRealSet.hpp"
#include "blocks/reals/BlockRealPlus.hpp"
#include "blocks/reals/BlockRealMinus.hpp"
#include "blocks/reals/BlockRealDivide.hpp"
#include "blocks/reals/BlockRealMultiply.hpp"
#include "blocks/reals/BlockRealMod.hpp"
#include "blocks/reals/BlockRealFloor.hpp"
#include "blocks/reals/BlockRealCeil.hpp"
#include "blocks/reals/BlockRealRound.hpp"
#include "blocks/reals/BlockRealFromBool.hpp"
#include "blocks/reals/BlockRealFromString.hpp"
#include "blocks/reals/BlockRealPi.hpp"
#include "blocks/reals/BlockRealEulers.hpp"
#include "blocks/reals/BlockRealRandom.hpp"
#include "blocks/reals/BlockRealLogarithmic.hpp"
#include "blocks/reals/BlockRealAbs.hpp"
#include "blocks/reals/BlockRealPow.hpp"
#include "blocks/reals/BlockRealLog.hpp"
#include "blocks/reals/BlockRealSin.hpp"
#include "blocks/reals/BlockRealCos.hpp"
#include "blocks/reals/BlockRealTan.hpp"
#include "blocks/reals/BlockRealCompEq.hpp"
#include "blocks/reals/BlockRealCompGr.hpp"
#include "blocks/reals/BlockRealCompLs.hpp"
#include "blocks/reals/BlockRealCompGrEq.hpp"
#include "blocks/reals/BlockRealCompLsEq.hpp"

// strings

#include "blocks/strings/BlockStringForce.hpp"
#include "blocks/strings/BlockStringLog.hpp"
#include "blocks/strings/BlockStringSubString.hpp"
#include "blocks/strings/BlockStringCharAt.hpp"
#include "blocks/strings/BlockStringCompEq.hpp"
#include "blocks/strings/BlockStringLength.hpp"
#include "blocks/strings/BlockStringSet.hpp"
#include "blocks/strings/BlockStringFromReal.hpp"
#include "blocks/strings/BlockStringFromRealInt.hpp"
#include "blocks/strings/BlockStringFromBool.hpp"
#include "blocks/strings/BlockStringFind.hpp"
#include "blocks/strings/BlockStringCatFrom.hpp"
*/

#include "blocks/strings/log.hpp"
#include "blocks/strings/set.hpp"

UB_EXPORT void Initialization(ModData* data)
{
	// ============================================
	// ==== Mod Information
	// ============================================

	data->ModUnlocalizedName = "ModVin";
	data->ModDisplayName = "Vanilla Mod";
	data->Version = "NULL";// TODO: version

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
	data->RegisterBlock(new BlockStringLog());
	data->RegisterBlock(new BlockStringSet());

	/*data->RegisterBlock(new BlockSystemSleep());
	data->RegisterBlock(new BlockSystemCreateWindow());
	data->RegisterBlock(new BlockSystemPullEvents());
	data->RegisterBlock(new BlockSystemWindowRender());
	data->RegisterBlock(new BlockSystemWindowVsync());
	data->RegisterBlock(new BlockSystemWindowFps());
	data->RegisterBlock(new BlockSystemWindowMouseX());
	data->RegisterBlock(new BlockSystemWindowMouseY());
	data->RegisterBlock(new BlockSystemWindowMouseLeft());
	data->RegisterBlock(new BlockSystemWindowMouseMiddle());
	data->RegisterBlock(new BlockSystemWindowMouseRight());
	data->RegisterBlock(new BlockSystemWindowPullScroll());

	// utility

	data->RegisterBlock(new BlockUtilityMarkPoint());
	data->RegisterBlock(new BlockUtilityGotoPoint());
	data->RegisterBlock(new BlockUtilityGotoPointIf());
	data->RegisterBlock(new BlockUtilityFunctionDefine());
	data->RegisterBlock(new BlockUtilityFunctionCall());
	data->RegisterBlock(new BlockUtilityThreadSummon());
	data->RegisterBlock(new BlockUtilityThreadKill());
	data->RegisterBlock(new BlockUtilityGetMicro());
	data->RegisterBlock(new BlockUtilityGlobalSetReal());
	data->RegisterBlock(new BlockUtilityGlobalGetReal());
	data->RegisterBlock(new BlockUtilityListCreate());
	data->RegisterBlock(new BlockUtilityListDestroy());
	data->RegisterBlock(new BlockUtilityListIndexReal());
	data->RegisterBlock(new BlockUtilityListIndexBool());
	data->RegisterBlock(new BlockUtilityListIndexString());
	data->RegisterBlock(new BlockUtilityListInsert());
	data->RegisterBlock(new BlockUtilityListRemove());
	data->RegisterBlock(new BlockUtilityListReplace());
	data->RegisterBlock(new BlockUtilityListReserve());
	data->RegisterBlock(new BlockUtilityListAdd());
	data->RegisterBlock(new BlockUtilityListSize());


	// graphics

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

	// booleans

	data->RegisterBlock(new BlockBooleanForce());
	data->RegisterBlock(new BlockBooleanLog());

	data->RegisterBlock(new BlockBooleanSet());
	data->RegisterBlock(new BlockBooleanNot());
	data->RegisterBlock(new BlockBooleanFlip());
	data->RegisterBlock(new BlockBooleanAnd());
	data->RegisterBlock(new BlockBooleanOr());

	// reals

	data->RegisterBlock(new BlockRealForce());
	data->RegisterBlock(new BlockRealLog());
	data->RegisterBlock(new BlockRealSet());
	data->RegisterBlock(new BlockRealFromBool());
	data->RegisterBlock(new BlockRealFromString());
	data->RegisterBlock(new BlockRealPi());
	data->RegisterBlock(new BlockRealEulers());
	data->RegisterBlock(new BlockRealPlus());
	data->RegisterBlock(new BlockRealMinus());
	data->RegisterBlock(new BlockRealDivide());
	data->RegisterBlock(new BlockRealMultiply());
	data->RegisterBlock(new BlockRealMod());
	data->RegisterBlock(new BlockRealRandom());
	data->RegisterBlock(new BlockRealPow());
	data->RegisterBlock(new BlockRealLogarithmic());
	data->RegisterBlock(new BlockRealFloor());
	data->RegisterBlock(new BlockRealCeil());
	data->RegisterBlock(new BlockRealRound());
	data->RegisterBlock(new BlockRealAbs());
	data->RegisterBlock(new BlockRealLog());
	data->RegisterBlock(new BlockRealSin());
	data->RegisterBlock(new BlockRealCos());
	data->RegisterBlock(new BlockRealTan());
	data->RegisterBlock(new BlockRealCompEq());
	data->RegisterBlock(new BlockRealCompGr());
	data->RegisterBlock(new BlockRealCompLs());
	data->RegisterBlock(new BlockRealCompGrEq());
	data->RegisterBlock(new BlockRealCompLsEq());

	// strings

	data->RegisterBlock(new BlockStringForce());
	data->RegisterBlock(new BlockStringLog());
	data->RegisterBlock(new BlockStringSet());
	data->RegisterBlock(new BlockStringCatFrom());
	data->RegisterBlock(new BlockStringFromReal());
	data->RegisterBlock(new BlockStringFromRealInt());
	data->RegisterBlock(new BlockStringFromBool());
	data->RegisterBlock(new BlockStringLength());
	data->RegisterBlock(new BlockStringCharAt());
	data->RegisterBlock(new BlockStringSubString());
	data->RegisterBlock(new BlockStringFind());
	data->RegisterBlock(new BlockStringCompEq());*/
}
