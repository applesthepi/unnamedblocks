#include "set.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_string(0) = pass->get_string(1);
//	l_output_5_23 = r_0;
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockStringSet::get_unlocalized_name() const
{
	return "vin_string_set";
}

const char* BlockStringSet::get_category() const
{
	return CATEGORY_STRINGS;
}

block_execution BlockStringSet::pull_execute_release() const
{
	return execute_release;
}

block_execution BlockStringSet::pull_execute_debug() const
{
	return execute_debug;
}

const std::vector<initializer> BlockStringSet::get_arguments() const
{
	std::vector<initializer> args;

	args.push_back(initializer(type::TEXT, variable_mode_restriction::NONE, variable_mode::RAW, "set"));
	args.push_back(initializer(type::STRING, variable_mode_restriction::RESTRICTED, variable_mode::VAR, "variable"));
	args.push_back(initializer(type::TEXT, variable_mode_restriction::NONE, variable_mode::RAW, "to"));
	args.push_back(initializer(type::STRING, variable_mode_restriction::NONE, variable_mode::RAW, ""));

	return args;
}
