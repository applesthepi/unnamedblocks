#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace esp
{
/// The esp::type is used in ubbs for the rust representation of a ub type.
class type
{
public:
	/// Example: "ub_modname_typename".
	virtual const char* get_unlocalized_name() = 0;

	/// Example: "bool = <<<>>>".
	virtual const char* get_ubbs_instance_r() = 0;

	/// Example: "bool = false".
	virtual const char* get_ubbs_instance_l() = 0;

	///
	virtual const char* get_ubbs_definition() = 0;
};
}