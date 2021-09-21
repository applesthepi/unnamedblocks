#include "log.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->log_info(pass->get_string(0));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* mod_vin::block::string::log::get_unlocalized_name() const
{
	return "vin_string_log";
}

const char* mod_vin::block::string::log::get_category() const
{
	return CATEGORY_STRINGS;
}

cap::mod::block::block::execution mod_vin::block::string::log::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::string::log::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::string::log::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "log" },
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}
