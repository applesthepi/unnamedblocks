#include "main.hpp"

static void execute_release(cap::mod::block::pass* pass)
{

}

static void execute_debug(cap::mod::block::pass* pass)
{
	
}

const char* mod_vin::block::system::main::get_unlocalized_name() const
{
	return "vin_main";
}

const char* mod_vin::block::system::main::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution mod_vin::block::system::main::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::system::main::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::system::main::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "main" }
	};
}
