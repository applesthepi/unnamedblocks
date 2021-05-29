#include "BlockRegistry.hpp"

#include <Cappuccino/Logger.h>

void BlockRegistry::RegisterCatagory(ModCatagory* catagory)
{
	m_catagories.push_back(catagory);
}

void BlockRegistry::RegisterBlock(ModBlock* block, const std::string& mod)
{
	m_blocks.push_back(block);
	m_blockMods.push_back(mod);
}

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

const std::vector<ModBlock*>& BlockRegistry::GetBlocks()
{
	return m_blocks;
}

const std::vector<ModCatagory*>& BlockRegistry::GetCategories()
{
	return m_catagories;
}

const std::string& BlockRegistry::GetBlockMod(const ModBlock* modBlock)
{
	for (uint64_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i] == modBlock)
			return m_blockMods[i];
	}

	Logger::Error("failed to get mod of block \"" + std::string(modBlock->GetUnlocalizedName()) + "\"");
	return m_blockMods.front();
}

BlockRegistry& BlockRegistry::GetRegistry()
{
	return m_registry;
}

BlockRegistry BlockRegistry::m_registry;
