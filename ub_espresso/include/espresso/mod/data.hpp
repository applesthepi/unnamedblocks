#pragma once
#include "config.h"

#include "espresso/mod/category.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/mod_block/block.hpp>

class ModDataBaked
{
public:
	u32 BlocksLength;
	ModBlock** Blocks;

	u32 CategoriesLength;
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