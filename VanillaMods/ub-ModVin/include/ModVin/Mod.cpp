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

	// strings

	data->RegisterBlock(new BlockStringForce());
	data->RegisterBlock(new BlockStringLog());
}
