#include "type.hpp"

esp::reg::type::type(esp::type& type)
{
	if (type.get_unlocalized_name() != nullptr)
		m_unlocalized_name = type.get_unlocalized_name();

	if (type.get_ubbs_instance() != nullptr)
		m_ubbs_instance = type.get_ubbs_instance();

	if (type.get_ubbs_definition() != nullptr)
		m_ubbs_definition = type.get_ubbs_definition();
}

const std::string& esp::reg::type::get_unlocalized_name()
{
	return m_unlocalized_name;
}

const std::string& esp::reg::type::get_ubbs_instance()
{
	return m_ubbs_instance;
}

const std::string& esp::reg::type::get_ubbs_definition()
{
	return m_ubbs_definition;
}
