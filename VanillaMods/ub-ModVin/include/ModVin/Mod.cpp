#include "Mod.h"

#include "blocks/BlockTest.h"
#include "blocks/BlockMain.h"
#include "blocks/BlockTestRender.h"
#include "blocks/BlockTestEvent.h"
#include "blocks/BlockYes.h"
#include "blocks/BlockYesTo.h"
#include "blocks/BlockBool.h"

UB_EXPORT void Initialization(ModData* data)
{
	data->ModUnlocalizedName = "ModVin";
	data->ModDisplayName = "Vanilla Mod";
	data->Version = ModVinVersion;

	// Categories

	data->RegisterCategory(new SomeCategory(data->ModUnlocalizedName, data->ModDisplayName));
	data->RegisterCategory(new SomeOtherCategory(data->ModUnlocalizedName, data->ModDisplayName));

	// Blocks

	data->RegisterBlock(new BlockTest());
	data->RegisterBlock(new BlockMain());
	data->RegisterBlock(new BlockTestRender());
	data->RegisterBlock(new BlockTestEvent());
	data->RegisterBlock(new BlockYes());
	data->RegisterBlock(new BlockYesTo());
	data->RegisterBlock(new BlockBool());
}
