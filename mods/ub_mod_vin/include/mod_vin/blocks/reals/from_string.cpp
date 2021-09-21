#include "from_string.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_real(0) = std::stod(pass->get_string(1));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	f64 nd = 0.0;

	try
	{
		nd = std::stod(pass->get_string(1));
	}
	catch (std::invalid_argument&)
	{
		pass->log_error("failed to set real \"" + std::to_string(pass->get_real(0)) + "\" to string \"" + pass->get_string(1) + "\"; the string could not be parsed into a real", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	pass->get_real(0) = nd;
}

const char* mod_vin::block::real::from_string::get_unlocalized_name() const
{
	return "vin_real_from_string";
}

const char* mod_vin::block::real::from_string::get_category() const
{
	return CATEGORY_REALS;
}

cap::mod::block::block::execution mod_vin::block::real::from_string::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::real::from_string::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::real::from_string::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "set" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "to" },
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}