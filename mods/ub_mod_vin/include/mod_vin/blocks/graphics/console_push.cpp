#include "console_push.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	cap::logger::info(cap::logger::level::RUNTIME, pass->get_string(0));
	cap::logger::info(cap::logger::level::RUNTIME, "TEST");
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* mod_vin::block::graphics::console_push::get_unlocalized_name() const
{
	return "vin_graphics_console_push";
}

const char* mod_vin::block::graphics::console_push::get_category() const
{
	return CATEGORY_GRAPHICS;
}

cap::mod::block::block::execution mod_vin::block::graphics::console_push::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::graphics::console_push::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::graphics::console_push::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "console push" },
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}