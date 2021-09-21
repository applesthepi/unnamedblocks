#include "BlockUtilityListAdd.hpp"
#include "aid/UtilityList.hpp"

#include <Cappuccino/Registration.hpp>
#include <vector>

static void execute_release(cap::mod::block::pass* pass)
{
	if (pass->get_bool(2))
		((UtilityList*)pass->CustomGet(pass->get_real(0)))->AddValue(new f64(pass->get_real(1)), true);
	else
		((UtilityList*)pass->CustomGet(pass->get_real(0)))->AddValue(&pass->get_real(1), false);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockUtilityListAdd::get_unlocalized_name() const
{
	return "vin_utility_list_add";
}

const char* BlockUtilityListAdd::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityListAdd::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityListAdd::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityListAdd::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "list"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "list"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "add"));
	{ cap::mod::block::block::argument::type::ANY, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "copy"));
	{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "1"));

	return args;
}
