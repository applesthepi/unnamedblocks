#include "BlockRegistry.h"
#include <Cappuccino/Logger.h>

BlockRegistry::BlockRegistry()
{

}

void BlockRegistry::RegisterCatagory(ModCatagory* catagory)
{
	m_catagories.push_back(catagory);
}

void BlockRegistry::RegisterBlock(ModBlock* block)
{
	m_blocks.push_back(block);
}
/*
void BlockRegistry::RegisterExeDebug(void(*fun)(ModBlockPass*))
{
	m_exeDebug.push_back(fun);
}

void BlockRegistry::RegisterExeRelease(void(*fun)(ModBlockPass*))
{
	m_exeRelease.push_back(fun);
}
*/
const ModBlock* BlockRegistry::GetBlock(const std::string& unlocalizedName)
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetUnlocalizedName() == unlocalizedName)
			return m_blocks[i];
	}

	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetUnlocalizedName() == std::string("vin_null"))
			return m_blocks[i];
	}

	Logger::Error("unexpected failure to receive vin_null block. Mod loading error?");
	return new ModBlock();
}

const ModCatagory* BlockRegistry::GetCategory(const std::string& unlocalizedName)
{
	for (unsigned int i = 0; i < m_catagories.size(); i++)
	{
		if (m_catagories[i]->GetUnlocalizedName() == unlocalizedName)
			return m_catagories[i];
	}

	Logger::Error("unexpected failure to get a catagory. Mod loading error?");
	return new ModCatagory("", "");
}

/*
executionFunction BlockRegistry::GetExeDebug(const std::string& blockUnlocalizedName)
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetUnlocalizedName() == blockUnlocalizedName)
			return m_exeDebug[i];
	}

	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetUnlocalizedName() == std::string("vin_null"))
			return m_exeDebug[i];
	}

	Logger::Error("FATAL! unexpected failure to receive vin_null block in attempt to receive execution function. Mod loading error?");
	return nullptr;
}

executionFunction BlockRegistry::GetExeRelease(const std::string& blockUnlocalizedName)
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetUnlocalizedName() == blockUnlocalizedName)
			return m_exeRelease[i];
	}

	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetUnlocalizedName() == std::string("vin_null"))
			return m_exeRelease[i];
	}

	Logger::Error("FATAL! unexpected failure to receive vin_null block in attempt to receive execution function. Mod loading error?");
	return nullptr;
}
*/
const std::vector<ModBlock*>& BlockRegistry::GetBlocks()
{
	return m_blocks;
}

const std::vector<ModCatagory*>& BlockRegistry::GetCategories()
{
	return m_catagories;
}
