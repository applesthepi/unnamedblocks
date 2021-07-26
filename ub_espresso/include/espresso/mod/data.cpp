#include "data.hpp"

ModDataBaked ModData::Bake()
{
	if (Blocks.size() == 0 || Categories.size() == 0)
	{
		Logger::Error("must have atleast 1 category and 1 block to bake!");
		return ModDataBaked();
	}

	ModDataBaked baked;

	baked.BlocksLength = Blocks.size();
	baked.CategoriesLength = Categories.size();

	baked.Blocks = (ModBlock**)malloc(sizeof(ModBlock*) * Blocks.size());
	baked.Categories = (ModCatagory**)malloc(sizeof(ModCatagory*) * Categories.size());

	if (baked.Blocks == nullptr || baked.Categories == nullptr)
	{
		printf("failed to allocate memory during baking process!\n");
		return baked;
	}

	for (u32 i = 0; i < Blocks.size(); i++)
		baked.Blocks[i] = Blocks[i];

	for (u32 i = 0; i < Categories.size(); i++)
		baked.Categories[i] = Categories[i];

	Blocks.clear();
	Categories.clear();

	return baked;
}

void ModData::RegisterBlock(ModBlock* block)
{
	Blocks.push_back(block);
}

void ModData::RegisterCategory(ModCatagory* category)
{
	Categories.push_back(category);
}
