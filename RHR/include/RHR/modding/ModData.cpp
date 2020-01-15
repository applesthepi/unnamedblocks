#include "ModData.h"

void ModData::RegisterBlock(const RegBlock block)
{
	m_blocks.push_back(block);
}

void ModData::RegisterCatagory(const RegCatagory catatory)
{
	m_catagories.push_back(catatory);
}

const std::vector<RegBlock>* ModData::GetBlocks()
{
	return &m_blocks;
}

const std::vector<RegCatagory>* ModData::GetCatagories()
{
	return &m_catagories;
}

