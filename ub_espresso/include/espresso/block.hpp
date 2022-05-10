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
	/// How the block will be identified as. Convention example: "mymod_some_block_name".
	virtual const char* get_unlocalized_name() = 0;

	/// Category of the block.
	virtual const char* get_category() = 0;

	/// Example: "println!("{}", <<<0>>>);".
	virtual const char* get_ubbs_instance() = 0;

	/// Does the block start a stack.
	virtual bool is_topical() = 0;

	///
	virtual std::vector<esp::argument::initializer> get_arguments() = 0;

	///
	const char* mod_unlocalized_name;
};
}