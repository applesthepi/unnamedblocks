#include "BlockUtilityListInsert.hpp"
#include "aid/UtilityList.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	if (pass->get_bool(3))
		((UtilityList*)pass->CustomGet(pass->get_real(0)))->InsertValue(new f64(pass->get_real(1)), pass->get_real(2), true);
	else
		((UtilityList*)pass->CustomGet(pass->get_real(0)))->InsertValue(&pass->get_real(1), pass->get_real(2), false);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	UtilityList* list = (UtilityList*)pass->CustomGet(pass->get_real(0));

	if (pass->get_real(2) >= list->Size() || pass->get_real(2) < 0 || std::floor(pass->get_real(2)) != pass->get_real(2))
	{
		pass->log_error("failed to insert index \"" + std::to_string(pass->get_real(1)) + "\" of a list; index is invalid", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	if (pass->get_bool(3))
		list->InsertValue(new f64(pass->get_real(1)), pass->get_real(2), true);
	else
		list->InsertValue(&pass->get_real(1), pass->get_real(2), false);
}

const char* BlockUtilityListInsert::get_unlocalized_name() const
{
	return "vin_utility_list_insert";
}

const char* BlockUtilityListInsert::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityListInsert::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityListInsert::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityListInsert::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "list"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "list"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "insert"));
	{ cap::mod::block::block::argument::type::ANY, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "at"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "0"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "copy"));
	{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "1"));

	return args;
}
