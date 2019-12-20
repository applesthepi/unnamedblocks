#include "BlockRegistry.h"

void BlockRegistry::Initialize()
{
	m_blocks = new std::vector<RegBlock>();
	m_catagories = new std::vector<RegCatagory>();
}

void BlockRegistry::CreateCatagory(RegCatagory catagory)
{
	m_catagories->push_back(catagory);
}

void BlockRegistry::CreateBlock(RegBlock block)
{
	m_blocks->push_back(block);
}

const RegBlock* BlockRegistry::GetBlock(std::string unlocalizedName)
{
	for (unsigned int i = 0; i < m_blocks->size(); i++)
	{
		if ((*m_blocks)[i].UnlocalizedName == unlocalizedName)
			return (const RegBlock*)(&(*m_blocks)[i]);
	}

	return nullptr;
}

const RegCatagory* BlockRegistry::GetCatagory(std::string unlocalizedName)
{
	for (unsigned int i = 0; i < m_catagories->size(); i++)
	{
		if ((*m_catagories)[i].UnlocalizedName == unlocalizedName)
			return (const RegCatagory*)(&(*m_catagories)[i]);
	}

	return nullptr;
}

std::vector<RegBlock>* BlockRegistry::GetBlocks()
{
	return m_blocks;
}

std::vector<RegCatagory>* BlockRegistry::GetCatagories()
{
	return m_catagories;
}

std::vector<RegBlock>* BlockRegistry::m_blocks;

std::vector<RegCatagory>* BlockRegistry::m_catagories;

void BlockArgument::SetupTEXT(std::string value)
{
	Type = BlockArgumentType::TEXT;
	Value = value;
}

void BlockArgument::SetupREAL(std::string value)
{
	Type = BlockArgumentType::REAL;
	Value = value;
}

void BlockArgument::SetupBOOL(std::string value)
{
	Type = BlockArgumentType::BOOL;
	Value = value;
}

void BlockArgument::SetupSTRING(std::string value)
{
	Type = BlockArgumentType::STRING;
	Value = value;
}
