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
	std::string category;

	///
	std::vector<std::string> positions;

	///
	std::vector<esp::argument*> argument_types;

	///
	std::vector<esp::argument::mode> argument_mode;

	///
	std::vector<esp::argument::mode_restriction> argument_restriction;
};
}