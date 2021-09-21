#include "log.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->log_info(pass->get_bool(0) ? "true" : "false");
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* mod_vin::block::boolean::log::get_unlocalized_name() const
{
	return "vin_boolean_log";
}

const char* mod_vin::block::boolean::log::get_category() const
{
	return CATEGORY_BOOLEANS;
}

cap::mod::block::block::execution mod_vin::block::boolean::log::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::boolean::log::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::boolean::log::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "log" },
		{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}
