#include "data.hpp"

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
