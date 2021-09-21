#include "from_bool.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_real(0) = pass->get_bool(1) ? 1 : 0;
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* mod_vin::block::real::from_bool::get_unlocalized_name() const
{
	return "vin_real_from_bool";
}

const char* mod_vin::block::real::from_bool::get_category() const
{
	return CATEGORY_REALS;
}

cap::mod::block::block::execution mod_vin::block::real::from_bool::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::real::from_bool::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::real::from_bool::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "set" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "to" },
		{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}