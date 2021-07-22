#include "BlockRegistry.hpp"

void BlockRegistry::RegisterBlock(ModBlock* block, const std::string& modUnlocalizedName)
{
	m_Blocks.push_back({ block, modUnlocalizedName });
}

void BlockRegistry::RegisterCatagory(ModCatagory* catagory, const std::string& modUnlocalizedName)
{
	m_Catagories.push_back({ catagory, modUnlocalizedName });
}

const BlockInfo* BlockRegistry::GetBlock(const std::string& unlocalizedName)
{
	for (auto& info : m_Blocks)
	{
		if (info.BlockModBlock->GetUnlocalizedName() == unlocalizedName)
			return &info;
	}

	for (auto& info : m_Blocks)
	{
		if (info.BlockModBlock->GetUnlocalizedName() == "vin_null")
			return &info;
	}

	Logger::Error("Unexpected failure to receive vin_null block. Mod loading error?");
	return nullptr;
}

const CatagoryInfo* BlockRegistry::GetCategory(const std::string& unlocalizedName)
{
	for (auto& info : m_Catagories)
	{
		if (info.CatagoryModCatagory->GetUnlocalizedName() == unlocalizedName)
			return &info;
	}

	Logger::Error("Unexpected failure to get a catagory. Mod loading error?");
	return nullptr;
}

const std::vector<BlockInfo>& BlockRegistry::GetBlocks()
{
	return m_Blocks;
}

const std::vector<CatagoryInfo>& BlockRegistry::GetCategories()
{
	return m_Catagories;
}

void BlockRegistry::CreateBlockRegistry()
{
	m_Registry = new BlockRegistry();
}

BlockRegistry& BlockRegistry::GetRegistry()
{
	return *m_Registry;
}

BlockRegistry* BlockRegistry::m_Registry;
