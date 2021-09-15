#include "log.hpp"

static void ExecuteRelease(pass* pass)
{
	pass->log_info(pass->get_string(0));
}

static void ExecuteDebug(pass* pass)
{
	execute_release(pass);
}

const char* BlockStringLog::get_unlocalized_name() const
{
	return "vin_string_log";
}

const char* BlockStringLog::get_category() const
{
	return CATEGORY_STRINGS;
}

block_execution BlockStringLog::pull_execute_release() const
{
	return execute_release;
}

block_execution BlockStringLog::pull_execute_debug() const
{
	return execute_debug;
}

const std::vector<initializer> BlockStringLog::get_arguments() const
{
	std::vector<initializer> args;

	args.push_back(initializer(type::TEXT, variable_mode_restriction::NONE, variable_mode::RAW, "log"));
	args.push_back(initializer(type::STRING, variable_mode_restriction::NONE, variable_mode::VAR, "variable"));

	return args;
}
