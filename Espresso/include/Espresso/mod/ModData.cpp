#include "ModData.h"
#include <string.h>

ModDataBaked ModData::Bake()
{
	ModDataBaked baked;

	baked.BlocksLength = m_blocks.size();
	baked.CategoriesLength = m_categories.size();

	baked.Blocks = (ModBlock*)malloc(sizeof(ModBlock) * m_blocks.size());
	baked.Categories = (ModCatagory*)malloc(sizeof(ModCatagory) * m_categories.size());

	if (baked.Blocks == nullptr || baked.Categories == nullptr)
	{
		printf("failed to allocate memory during baking process!\n");
		return baked;
	}

	memcpy(baked.Blocks, m_blocks.data(), m_blocks.size());
	memcpy(baked.Categories, m_categories.data(), m_categories.size());

	for (uint32_t i = 0; i < m_blocks.size(); i++)
		delete m_blocks[i];

	for (uint32_t i = 0; i < m_categories.size(); i++)
		delete m_categories[i];

	m_blocks.clear();
	m_categories.clear();
}

void ModData::RegisterBlock(ModBlock* block)
{
	m_blocks.push_back(block);
}

void ModData::RegisterCategory(ModCatagory* category)
{
	m_categories.push_back(category);
}
