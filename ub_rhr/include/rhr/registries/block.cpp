#include "block.hpp"

void rhr::registry::block::register_block(ModBlock* block, const std::string& mod_unlocalized_name)
{
	m_blocks.push_back({ block, mod_unlocalized_name });
}

void rhr::registry::block::register_category(ModCatagory* catagory, const std::string& mod_unlocalized_name)
{
	m_catagories.push_back({ catagory, mod_unlocalized_name });
}

const rhr::registry::block::BlockInfo* rhr::registry::block::get_block(const std::string& unlocalized_name)
{
	for (auto& info : m_blocks)
	{
		if (info.BlockModBlock->GetUnlocalizedName() == unlocalized_name)
			return &info;
	}

	for (auto& info : m_blocks)
	{
		if (info.BlockModBlock->GetUnlocalizedName() == "vin_null")
			return &info;
	}

	Logger::Error("Unexpected failure to receive vin_null block. Mod loading error?");
	return nullptr;
}

const rhr::registry::block::CatagoryInfo* rhr::registry::block::get_categories(const std::string& unlocalized_name)
{
	for (auto& info : m_catagories)
	{
		if (info.CatagoryModCatagory->GetUnlocalizedName() == unlocalized_name)
			return &info;
	}

	Logger::Error("Unexpected failure to get a catagory. Mod loading error?");
	return nullptr;
}

const std::vector<rhr::registry::block::BlockInfo>& rhr::registry::block::get_blocks()
{
	return m_blocks;
}

const std::vector<rhr::registry::block::CatagoryInfo>& rhr::registry::block::get_categories()
{
	return m_catagories;
}

void rhr::registry::block::create_block_registry()
{
	m_registry = new rhr::registry::block();
}

rhr::registry::block& rhr::registry::block::get_registry()
{
	return *m_registry;
}

rhr::registry::block* rhr::registry::block::m_registry;
