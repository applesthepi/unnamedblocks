#include "data.hpp"

void esp::mod::data::register_block(espresso::mod::block::block* block)
{
	m_blocks.push_back(block);
}

void esp::mod::data::register_category(esp::mod::category* category)
{
	m_categories.push_back(category);
}

const std::vector<espresso::mod::block::block*>& esp::mod::data::get_blocks()
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

cap::logger::stream* esp::mod::data::get_stream_system()
{
	return m_stream_system;
}

cap::logger::stream* esp::mod::data::get_stream_editor()
{
	return m_stream_editor;
}

cap::logger::stream* esp::mod::data::get_stream_runtime()
{
	return m_stream_runtime;
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
	cap::logger::stream* stream_system, cap::logger::stream* stream_editor, cap::logger::stream* stream_runtime)
{
	m_stream_system	 = stream_system;
	m_stream_editor	 = stream_editor;
	m_stream_runtime = stream_runtime;
}
