#include "data.hpp"

//ModDataBaked ModData::Bake()
//{
//	if (Blocks.size() == 0 || Categories.size() == 0)
//	{
//		cap::logger::Error("must have atleast 1 category and 1 block to bake!");
//		return ModDataBaked();
//	}
//
//	ModDataBaked baked;
//
//	baked.BlocksLength = Blocks.size();
//	baked.CategoriesLength = Categories.size();
//
//	baked.Blocks = (ModBlock**)malloc(sizeof(ModBlock*) * Blocks.size());
//	baked.Categories = (esp::mod::category**)malloc(sizeof(esp::mod::category*) * Categories.size());
//
//	if (baked.Blocks == nullptr || baked.Categories == nullptr)
//	{
//		printf("failed to allocate memory during baking process!\n");
//		return baked;
//	}
//
//	for (u32 i = 0; i < Blocks.size(); i++)
//		baked.Blocks[i] = Blocks[i];
//
//	for (u32 i = 0; i < Categories.size(); i++)
//		baked.Categories[i] = Categories[i];
//
//	Blocks.clear();
//	Categories.clear();
//
//	return baked;
//}

void esp::mod::data::register_block(cap::mod::block::block* block)
{
	m_blocks.push_back(block);
}

void esp::mod::data::register_category(esp::mod::category* category)
{
	m_categories.push_back(category);
}

const std::vector<cap::mod::block::block*>& esp::mod::data::get_blocks()
{
	return m_blocks;
}

const std::vector<esp::mod::category*>& esp::mod::data::get_categories()
{
	return m_categories;
}

const std::string& esp::mod::data::get_mod_display_name()
{
	return m_mod_display_name;
}

const std::string& esp::mod::data::get_mod_unlocalized_name()
{
	return m_mod_unlocalized_name;
}

const std::string& esp::mod::data::get_version()
{
	return m_version;
}
