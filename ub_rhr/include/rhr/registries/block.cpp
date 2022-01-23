#include "block.hpp"

void rhr::registry::block::register_block(cap::mod::block::block* block, const std::string& mod_unlocalized_name) { m_blocks.push_back({block, mod_unlocalized_name}); }

void rhr::registry::block::register_category(esp::mod::category* category, const std::string& mod_unlocalized_name) { m_categories.push_back({category, mod_unlocalized_name}); }

const rhr::registry::block::block_info* rhr::registry::block::get_block(const std::string& unlocalized_name)
{
	//	cap::logger::info("block registry get block: " + unlocalized_name);

	for (auto& info : m_blocks)
	{
		//		cap::logger::info("block registry get block --- testing block: " + info.block_mod_unlocalized_name);

		if (info.block_mod_block->get_unlocalized_name() == unlocalized_name)
		{
			//			cap::logger::info("block registry get block --- FOUND block: " + info.block_mod_unlocalized_name);
			return &info;
		}
	}

	for (auto& info : m_blocks)
	{
		if (info.block_mod_block->get_unlocalized_name() == "vin_null")
			return &info;
	}

	cap::logger::error(cap::logger::level::SYSTEM, "Unexpected failure to receive vin_null block. Mod loading error?");
	return nullptr;
}

const rhr::registry::block::category_info* rhr::registry::block::get_categories(const std::string& unlocalized_name)
{
	for (auto& info : m_categories)
	{
		if (info.category_mod_category->get_unlocalized_name() == unlocalized_name)
			return &info;
	}

	cap::logger::error(cap::logger::level::SYSTEM, "Unexpected failure to get a catagory. Mod loading error?");
	return nullptr;
}

const std::vector<rhr::registry::block::block_info>& rhr::registry::block::get_blocks() { return m_blocks; }

const std::vector<rhr::registry::block::category_info>& rhr::registry::block::get_categories() { return m_categories; }

void rhr::registry::block::create_block_registry() { m_registry = new rhr::registry::block(); }

rhr::registry::block& rhr::registry::block::get_registry() { return *m_registry; }

rhr::registry::block* rhr::registry::block::m_registry;
