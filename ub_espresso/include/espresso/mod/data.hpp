#pragma once
#include "config.h"

#include "espresso/type.hpp"
#include "espresso/argument.hpp"
#include "espresso/block.hpp"
#include "espresso/registry.hpp"

// TODO: get rid of these
#include "espresso/mod/block/block.hpp"
#include "espresso/mod/category.hpp"

#include <latte/utils.hpp>

namespace esp::mod
{
///
class data
{
public:
	///
	void register_type(esp::type* type);

	///
	void register_argument(esp::argument* argument);

	///
	void register_block(esp::block* block);

	///
	void register_category(esp::mod::category* category);

	///
	const std::vector<esp::type*>& get_types();

	///
	const std::vector<esp::argument*>& get_arguments();

	///
	const std::vector<esp::block*>& get_blocks();

	///
	const std::vector<esp::mod::category*>& get_categories();

	///
	const std::string& get_mod_display_name();

	///
	const std::string& get_mod_unlocalized_name();

	///
	const std::string& get_version();

	///
	latte::logger::stream* get_stream_system();

	///
	latte::logger::stream* get_stream_editor();

	///
	latte::logger::stream* get_stream_runtime();

	///
	esp::registry* get_registry();

	///
	void set_mod_display_name(const std::string& display_name);

	///
	void set_mod_unlocalized_name(const std::string& unlocalized_name);

	///
	void set_version(const std::string& version);

	///
	void set_logger_linkage(
		latte::logger::stream* stream_system, latte::logger::stream* stream_editor, latte::logger::stream* stream_runtime);

	///
	void set_registry(esp::registry* registry);

private:
	///
	std::vector<esp::type*> m_types;

	///
	std::vector<esp::argument*> m_arguments;

	///
	std::vector<esp::block*> m_blocks;

	///
	std::vector<esp::mod::category*> m_categories;

	///
	std::string m_mod_display_name;

	///
	std::string m_mod_unlocalized_name;

	///
	std::string m_version;

	///
	latte::logger::stream* m_stream_system;

	///
	latte::logger::stream* m_stream_editor;

	///
	latte::logger::stream* m_stream_runtime;

	///
	esp::registry* m_registry;
};
} // namespace esp::mod