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
	virtual const char* get_unlocalized_name();

	/// Example: "bool = false".
	virtual const char* get_ubbs_instance();

	///
	virtual const char* get_ubbs_definition();
};
}