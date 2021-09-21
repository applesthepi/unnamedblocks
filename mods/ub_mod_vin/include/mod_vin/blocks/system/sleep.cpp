#include "BlockSystemSleep.hpp"
#include <thread>
#include <chrono>
#include <cmath>

static void execute_release(cap::mod::block::pass* pass)
{
	std::this_thread::sleep_for(std::chrono::milliseconds((u64)pass->get_real(0)));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	f64 ms = pass->get_real(0);

	if (std::floor(ms) != ms)
	{
		pass->log_error("failed to sleep for \"" + std::to_string(ms) + "\" seconds; milliseconds is a decimal", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	i64 msInt = ms;

	if (msInt < 0)
	{
		pass->log_error("cannot sleep for \"" + std::to_string(msInt) + "\" milliseconds; milliseconds is negitive", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	u32 msUint = ms;

	std::this_thread::sleep_for(std::chrono::milliseconds(msUint));
}

const char* BlockSystemSleep::get_unlocalized_name() const
{
	return "vin_system_sleep";
}

const char* BlockSystemSleep::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemSleep::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution BlockSystemSleep::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemSleep::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "sleep"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "1000"));

	return args;
}
