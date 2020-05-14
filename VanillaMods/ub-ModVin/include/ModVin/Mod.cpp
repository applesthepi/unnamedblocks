#include "Mod.h"

// system

#include "blocks/system/BlockSystemMain.h"

// objects

// booleans

#include "blocks/booleans/BlockBooleanForce.h"
#include "blocks/booleans/BlockBooleanLog.h"

#include "blocks/booleans/BlockBooleanSet.h"
#include "blocks/booleans/BlockBooleanFlip.h"
#include "blocks/booleans/BlockBooleanAnd.h"
#include "blocks/booleans/BlockBooleanOr.h"
#include "blocks/booleans/BlockBooleanNot.h"

// reals

#include "blocks/reals/BlockRealForce.h"
#include "blocks/reals/BlockRealLog.h"
#include "blocks/reals/BlockRealSet.h"
#include "blocks/reals/BlockRealPlus.h"
#include "blocks/reals/BlockRealMinus.h"
#include "blocks/reals/BlockRealDivide.h"
#include "blocks/reals/BlockRealMultiply.h"
#include "blocks/reals/BlockRealMod.h"
#include "blocks/reals/BlockRealFloor.h"
#include "blocks/reals/BlockRealCeil.h"
#include "blocks/reals/BlockRealRound.h"
#include "blocks/reals/BlockRealFromBool.h"
#include "blocks/reals/BlockRealFromString.h"
#include "blocks/reals/BlockRealPi.h"
#include "blocks/reals/BlockRealEulers.h"
#include "blocks/reals/BlockRealRandom.h"
#include "blocks/reals/BlockRealAbs.h"
#include "blocks/reals/BlockRealPow.h"
#include "blocks/reals/BlockRealLog.h"
#include "blocks/reals/BlockRealSin.h"
#include "blocks/reals/BlockRealCos.h"
#include "blocks/reals/BlockRealTan.h"
#include "blocks/reals/BlockRealCompEq.h"
#include "blocks/reals/BlockRealCompGr.h"
#include "blocks/reals/BlockRealCompLs.h"
#include "blocks/reals/BlockRealCompGrEq.h"
#include "blocks/reals/BlockRealCompLsEq.h"

// strings

#include "blocks/strings/BlockStringForce.h"
#include "blocks/strings/BlockStringLog.h"

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
	data->RegisterCategory(new CategoryObjects(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryBooleans(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryReals(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new CategoryStrings(data->ModUnlocalizedName, data->ModDisplayName));

	// ============================================
	// ==== Blocks
	// ============================================

	// system

	data->RegisterBlock(new BlockSystemMain());

	// objects

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
	data->RegisterBlock(new BlockRealFloor());
	data->RegisterBlock(new BlockRealCeil());
	data->RegisterBlock(new BlockRealRound());
	data->RegisterBlock(new BlockRealRandom());
	data->RegisterBlock(new BlockRealAbs());
	data->RegisterBlock(new BlockRealPow());
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
}
