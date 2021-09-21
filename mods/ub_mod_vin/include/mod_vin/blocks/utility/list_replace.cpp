#include "BlockUtilityListReplace.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->get_real(0));
	(*list)[pass->get_real(1)] = &pass->get_real(2);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->get_real(0));

	if (pass->get_real(1) >= list->size() || pass->get_real(1) < 0 || std::floor(pass->get_real(1)) != pass->get_real(1))
	{
		pass->log_error("failed to replace index \"" + std::to_string(pass->get_real(1)) + "\" of a list; index is invalid", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	(*list)[pass->get_real(1)] = &pass->get_real(2);
}

const char* BlockUtilityListReplace::get_unlocalized_name() const
{
	return "vin_utility_list_replace";
}

const char* BlockUtilityListReplace::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityListReplace::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityListReplace::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityListReplace::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "list"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "list"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "replace"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "0"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "with"));
	{ cap::mod::block::block::argument::type::ANY, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "copy"));
	{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "1"));

	return args;
}
