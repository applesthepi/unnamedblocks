#pragma once
#include "config.h"

#include "catagory/ModCatagory.hpp"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/block/ModBlock.hpp>

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