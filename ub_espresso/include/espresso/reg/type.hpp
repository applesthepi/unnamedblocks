#pragma once
#include "config.h"

#include "espresso/type.hpp"

#include <latte/utils.hpp>

namespace esp::reg
{
///
class type
{
public:
	explicit type(esp::type& type);

	///
	const std::string& get_unlocalized_name();

	///
	const std::string& get_ubbs_instance();

	///
	const std::string& get_ubbs_definition();
private:
	///
	std::string m_unlocalized_name;

	///
	std::string m_ubbs_instance;

	///
	std::string m_ubbs_definition;
};
}