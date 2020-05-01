#pragma once
#include "Espresso/block/ModBlock.h"
#include "Espresso/catagory/ModCatagory.h"

class ModDataBaked
{
public:
	uint32_t BlocksLength;
	ModBlock** Blocks;

	uint32_t CategoriesLength;
	ModCatagory** Categories;
};

class ModData
{
public:
	ModDataBaked Bake();

	void RegisterBlock(ModBlock* block);
	void RegisterCategory(ModCatagory* category);

	std::vector<ModBlock*> Blocks;
	std::vector<ModCatagory*> Categories;
	std::string ModDisplayName;
	std::string ModUnlocalizedName;
	std::string Version;
};