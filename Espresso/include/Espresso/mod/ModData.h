#pragma once
#include "Espresso/block/ModBlock.h"
#include "Espresso/catagory/ModCatagory.h"

class ModDataBaked
{
public:
	uint32_t BlocksLength;
	ModBlock* Blocks;

	uint32_t CategoriesLength;
	ModCatagory* Categories;
};

class ModData
{
public:
	ModDataBaked Bake();

	void RegisterBlock(ModBlock* block);
	void RegisterCategory(ModCatagory* category);
private:
	std::vector<ModBlock*> m_blocks;
	std::vector<ModCatagory*> m_categories;
};