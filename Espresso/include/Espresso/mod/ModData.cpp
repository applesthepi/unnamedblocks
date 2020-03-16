#include "ModData.h"
#include <string.h>

ModDataBaked ModData::Bake()
{
	if (m_blocks.size() == 0 || m_categories.size() == 0)
	{
		Logger::Error("must have atleast 1 category and 1 block to bake!");
		return ModDataBaked();
	}

	ModDataBaked baked;

	baked.BlocksLength = m_blocks.size();
	baked.CategoriesLength = m_categories.size();

	baked.Blocks = (ModBlock**)malloc(sizeof(ModBlock*) * m_blocks.size());
	baked.Categories = (ModCatagory**)malloc(sizeof(ModCatagory*) * m_categories.size());

	if (baked.Blocks == nullptr || baked.Categories == nullptr)
	{
		printf("failed to allocate memory during baking process!\n");
		return baked;
	}

	for (uint32_t i = 0; i < m_blocks.size(); i++)
		baked.Blocks[i] = m_blocks[i];

	for (uint32_t i = 0; i < m_categories.size(); i++)
		baked.Categories[i] = m_categories[i];

	std::cout << baked.Categories[0]->GetDisplayName() << std::endl;
	std::cout << m_categories[0]->GetDisplayName() << std::endl;

	m_blocks.clear();
	m_categories.clear();

	return baked;
}

void ModData::RegisterBlock(ModBlock* block)
{
	m_blocks.push_back(block);
}

void ModData::RegisterCategory(ModCatagory* category)
{
	std::cout << "registering: " << category->GetDisplayName() << std::endl;
	m_categories.push_back(category);
}
