#include "main.hpp"

static void ExecuteRelease(pass* pass)
{

}

static void ExecuteDebug(pass* pass)
{
	
}

const char* BlockSystemMain::get_unlocalized_name() const
{
	return "vin_main";
}

const char* BlockSystemMain::get_category() const
{
	return CATEGORY_SYSTEM;
}

bool BlockSystemMain::is_topical() const
{
	return true;
}

block_execution BlockSystemMain::pull_execute_release() const
{
	return execute_release;
}

block_execution BlockSystemMain::pull_execute_debug() const
{
	return execute_debug;
}

const std::vector<initializer> BlockSystemMain::get_arguments() const
{
	std::vector<initializer> args;

	args.push_back(initializer(type::TEXT, variable_mode_restriction::NONE, variable_mode::RAW, "main"));

	return args;
}
