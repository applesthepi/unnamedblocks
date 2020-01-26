#include "ModData.h"

ModData::ModData()
{

}

void ModData::RegisterBlock(const RegBlock block)
{
	m_blocks.push_back(block);
}

void ModData::RegisterCatagory(const RegCatagory catatory)
{
	m_catagories.push_back(catatory);
}

std::vector<RegBlock>* ModData::GetBlocks()
{
	return &m_blocks;
}

std::vector<RegCatagory>* ModData::GetCatagories()
{
	return &m_catagories;
}

