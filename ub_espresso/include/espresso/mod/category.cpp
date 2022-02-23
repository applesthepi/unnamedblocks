#include "category.hpp"

esp::mod::category::category(const std::string& mod_unlocalized_name, const std::string& mod_display_name)
	: m_mod_unlocalized_name(mod_unlocalized_name)
	, m_mod_display_name(mod_display_name)
{}

std::string esp::mod::category::get_mod_unlocalized_name()
{
	return m_mod_unlocalized_name;
}

std::string esp::mod::category::get_mod_display_name()
{
	return m_mod_display_name;
}

espresso::color esp::mod::category::get_color() const
{
	return espresso::color().from_u8({255, 0, 255, 255});
}

std::string esp::mod::category::get_unlocalized_name() const
{
	return "NULL";
}

std::string esp::mod::category::get_display_name() const
{
	return "NULL";
}
