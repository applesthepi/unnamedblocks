#include "BlockUtilityGetMicro.hpp"

#include <Cappuccino/Registration.hpp>
#include <chrono>

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_real(0) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - *pass->GetBeginTime()).count();
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockUtilityGetMicro::get_unlocalized_name() const
{
	return "vin_utility_get_micro";
}

const char* BlockUtilityGetMicro::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityGetMicro::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityGetMicro::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityGetMicro::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "get micro"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "micro"));

	return args;
}
