#include "data.hpp"

void esp::mod::data::register_type(esp::type* type)
{
	m_types.push_back(type);
}

void esp::mod::data::register_argument(esp::argument* argument)
{
	m_arguments.push_back(argument);
}

void esp::mod::data::register_block(esp::block* block)
{
	m_blocks.push_back(block);
}

void esp::mod::data::register_category(esp::mod::category* category)
{
	m_categories.push_back(category);
}

const std::vector<esp::type*>& esp::mod::data::get_types()
{
	return m_types;
}

const std::vector<esp::argument*>& esp::mod::data::get_arguments()
{
	return m_arguments;
}

const std::vector<esp::block*>& esp::mod::data::get_blocks()
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

latte::logger::stream* esp::mod::data::get_stream_system()
{
	return m_stream_system;
}

latte::logger::stream* esp::mod::data::get_stream_editor()
{
	return m_stream_editor;
}

latte::logger::stream* esp::mod::data::get_stream_runtime()
{
	return m_stream_runtime;
}

esp::registry* esp::mod::data::get_registry()
{
	return m_registry;
}

void esp::mod::data::set_mod_display_name(const std::string& display_name)
{
	m_mod_display_name = display_name;
}

void esp::mod::data::set_mod_unlocalized_name(const std::string& unlocalized_name)
{
	m_mod_unlocalized_name = unlocalized_name;
}

void esp::mod::data::set_version(const std::string& version)
{
	m_version = version;
}

void esp::mod::data::set_logger_linkage(
	latte::logger::stream* stream_system, latte::logger::stream* stream_editor, latte::logger::stream* stream_runtime)
{
	m_stream_system	 = stream_system;
	m_stream_editor	 = stream_editor;
	m_stream_runtime = stream_runtime;
}

void esp::mod::data::set_registry(esp::registry* registry)
{
	m_registry = registry;
}
