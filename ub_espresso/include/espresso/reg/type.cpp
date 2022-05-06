#include "type.hpp"

esp::reg::type::type(esp::type* type)
{
	if (type->get_unlocalized_name() != nullptr)
		m_unlocalized_name = type->get_unlocalized_name();

	if (type->get_ubbs_instance_r() != nullptr)
		m_ubbs_instance_r = type->get_ubbs_instance_r();

	if (type->get_ubbs_instance_l() != nullptr)
		m_ubbs_instance_l = type->get_ubbs_instance_l();

	if (type->get_ubbs_definition() != nullptr)
		m_ubbs_definition = type->get_ubbs_definition();
}

const std::string& esp::reg::type::get_unlocalized_name()
{
	return m_unlocalized_name;
}

const std::string& esp::reg::type::get_ubbs_instance_r()
{
	return m_ubbs_instance_r;
}

const std::string& esp::reg::type::get_ubbs_instance_l()
{
	return m_ubbs_instance_l;
}

const std::string& esp::reg::type::get_ubbs_definition()
{
	return m_ubbs_definition;
}
