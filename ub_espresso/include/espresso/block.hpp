#pragma once
#include "config.h"

#include "espresso/argument.hpp"

#include <latte/utils.hpp>

namespace esp
{
///
struct block
{
	///
	std::string unlocalized_name;

	///
	std::string category;

	///
	std::vector<std::string> positions;

	///
	std::vector<std::string> argument_types;

	///
	std::vector<esp::argument::mode> argument_mode;

	///
	std::vector<esp::argument::mode_restriction> argument_restriction;

	///
	std::vector<std::string> argument_defaults;
};
}