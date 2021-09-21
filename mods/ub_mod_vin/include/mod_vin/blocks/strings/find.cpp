#include "find.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	usize result = pass->get_string(0).find(pass->get_string(1), pass->get_real(2));

	if (result == std::string::npos)
		pass->get_real(3) = -1;
	else
		pass->get_real(3) = result;
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* mod_vin::block::string::find::get_unlocalized_name() const
{
	return "vin_string_find";
}

const char* mod_vin::block::string::find::get_category() const
{
	return CATEGORY_STRINGS;
}

cap::mod::block::block::execution mod_vin::block::string::find::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::string::find::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::string::find::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "find" },
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "after" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "0" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "for" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}
