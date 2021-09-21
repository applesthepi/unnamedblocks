#include "BlockUtilityListReserve.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->get_real(0));
	list->reserve(pass->get_real(1));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->get_real(0));

	if (pass->get_real(1) >= list->size() || pass->get_real(1) < 0 || std::floor(pass->get_real(1)) != pass->get_real(1))
	{
		pass->log_error("failed to reserve \"" + std::to_string(pass->get_real(1)) + "\" capacity of a list; index is invalid", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	list->reserve(pass->get_real(1));
}

const char* BlockUtilityListReserve::get_unlocalized_name() const
{
	return "vin_utility_list_reserve";
}

const char* BlockUtilityListReserve::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityListReserve::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityListReserve::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityListReserve::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "list"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "list"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "reserve"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "1"));

	return args;
}
