#pragma once
#include "config.h"

#include "espresso/argument.hpp"

#include <latte/utils.hpp>

namespace esp
{
///
class block
{
public:
	///
	virtual std::string get_mod_unlocalized_name() = 0;

	/// How the block will be identified as. Convention example: "mymod_some_block_name".
	virtual const char* get_unlocalized_name() = 0;

	/// Category of the block.
	virtual const char* get_category() = 0;

	/// Does the block start a stack.
	virtual bool is_topical() = 0;

	///
	virtual std::vector<esp::argument::initializer> get_arguments() = 0;
};
}