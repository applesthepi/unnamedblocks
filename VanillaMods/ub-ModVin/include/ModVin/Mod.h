#pragma once
#include "config.h"
#include "ModCategories.h"

#include "blocks/BlockPrint.h"

#include <Espresso/Espresso.h>
#include <cmath>

#ifdef LINUX
#define UB_EXPORT extern "C"
#else
#define UB_EXPORT extern "C" __declspec(dllexport)
#endif

UB_EXPORT void Initialization(ModData* data)
{
	data->ModFileName = "ModVin";
	data->ModDisplayName = "Vanilla Mod";
	data->Version = ModVinVersion;

	// Categories

	data->RegisterCategory(new SomeCategory());
	
	// Blocks
	
	data->RegisterBlock(new SomeBlock());
}
