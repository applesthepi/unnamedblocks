#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <cappuccino/mod_block/block.hpp>
#include <cappuccino/execution_thread.hpp>
#include <espresso/mod/category.hpp>

namespace rhr::registry
{
// TODO: make completely static and remove singleton
///
class block
{
public:
	// TODO: fix location/case
	///
	class BlockArgumentCaller
	{
	public:
		BlockArgumentCaller(const BlockArgumentVariableMode mode, const std::string value)
			:Mode(mode), Value(value) {}

		BlockArgumentVariableMode Mode;
		std::string Value;
	};

	// TODO: fix location/case
	///
	struct BlockInfo
	{
		ModBlock* BlockModBlock;
		std::string BlockModUnlocalizedName;
	};

	// TODO: fix location/case
	///
	struct CatagoryInfo
	{
		ModCatagory* CatagoryModCatagory;
		std::string CatagoryModUnlocalizedName;
	};

	///
	void register_block(ModBlock* block, const std::string& mod_unlocalized_name);

	///
	void register_category(ModCatagory* catagory, const std::string& mod_unlocalized_name);

	///
	const BlockInfo* get_block(const std::string& unlocalized_name);

	///
	const CatagoryInfo* get_categories(const std::string& unlocalized_name);

	///
	const std::vector<BlockInfo>& get_blocks();

	///
	const std::vector<CatagoryInfo>& get_categories();

	///
	static void create_block_registry();

	///
	static block& get_registry();
private:
	///
	static block* m_registry;

	///
	std::vector<BlockInfo> m_blocks;

	///
	std::vector<CatagoryInfo> m_catagories;
};
}
