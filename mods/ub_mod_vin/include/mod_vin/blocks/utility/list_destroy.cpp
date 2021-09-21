#include "BlockUtilityListDestroy.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	pass->CustomFree(pass->get_real(0));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockUtilityListDestroy::get_unlocalized_name() const
{
	return "vin_utility_list_destroy";
}

const char* BlockUtilityListDestroy::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityListDestroy::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityListDestroy::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityListDestroy::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "list destroy"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "list"));

	return args;
}
