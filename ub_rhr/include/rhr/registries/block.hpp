#pragma once
#include "config.h"

#include <cappuccino/execution_thread.hpp>
#include <cappuccino/mod/block/block.hpp>
#include <cappuccino/utils.hpp>
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
	class block_argument_caller
	{
	public:
		block_argument_caller(const cap::mod::block::block::argument::variable_mode mode, const std::string& value)
			: m_mode(mode)
			, m_value(value)
		{}

		///
		cap::mod::block::block::argument::variable_mode get_mode()
		{
			return m_mode;
		};

		///
		const std::string& get_value()
		{
			return m_value;
		};

	private:
		///
		cap::mod::block::block::argument::variable_mode m_mode;

		///
		std::string m_value;
	};

	// TODO: fix location/case
	///
	struct block_info
	{
		cap::mod::block::block* block_mod_block;
		std::string block_mod_unlocalized_name;
	};

	// TODO: fix location/case
	///
	struct category_info
	{
		esp::mod::category* category_mod_category;
		std::string category_mod_unlocalized_name;
	};

	///
	void register_block(cap::mod::block::block* block, const std::string& mod_unlocalized_name);

	///
	void register_category(esp::mod::category* category, const std::string& mod_unlocalized_name);

	///
	const block_info* get_block(const std::string& unlocalized_name);

	///
	const category_info* get_categories(const std::string& unlocalized_name);

	///
	const std::vector<block_info>& get_blocks();

	///
	const std::vector<category_info>& get_categories();

	///
	static void create_block_registry();

	///
	static block& get_registry();

private:
	///
	static block* m_registry;

	///
	std::vector<block_info> m_blocks;

	///
	std::vector<category_info> m_categories;
};
} // namespace rhr::registry
