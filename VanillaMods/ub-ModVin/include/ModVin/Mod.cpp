
#include "Mod.h"

UB_EXPORT void Initialization(ModData* data)
{
	data->ModFileName = "ModVin";
	data->ModDisplayName = "Vanilla Mod";
	data->Version = ModVinVersion;

	// Categories

	data->RegisterCategory(new SomeCategory());

	// Blocks

	data->RegisterBlock(new BlockTest());
	data->RegisterBlock(new BlockMain());
	data->RegisterBlock(new BlockTestRender());
	data->RegisterBlock(new BlockTestEvent());
	data->RegisterBlock(new BlockYes());
}
