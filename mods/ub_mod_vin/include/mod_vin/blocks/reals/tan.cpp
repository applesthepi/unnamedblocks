#include "tan.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_real(0) = std::tan(pass->get_real(0) * (3.141592653589793 / 180.0));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* mod_vin::block::real::tan::get_unlocalized_name() const
{
	return "vin_real_tan";
}

const char* mod_vin::block::real::tan::get_category() const
{
	return CATEGORY_REALS;
}

cap::mod::block::block::execution mod_vin::block::real::tan::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::real::tan::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::real::tan::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "tan" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}