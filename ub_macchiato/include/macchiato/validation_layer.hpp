#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac
{
///
struct validation_layer
{
	///
	enum class importance
	{
		REQUIRED, OPTIONAL
	};

	///
	static void simplify(const std::vector<mac::validation_layer>& in, std::vector<const char*>& out);

	///
	std::string name;

	///
	mac::validation_layer::importance validation_layer_importance;
};
}