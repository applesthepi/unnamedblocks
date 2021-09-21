#include "BlockUtilityListCreate.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_real(0) = pass->CustomPut(new std::vector<void*>());
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockUtilityListCreate::get_unlocalized_name() const
{
	return "vin_utility_list_create";
}

const char* BlockUtilityListCreate::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityListCreate::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityListCreate::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityListCreate::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "list create"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "list"));

	return args;
}
