#include "BlockSystemWindowVsync.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void execute_release(cap::mod::block::pass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->get_real(0));
	//handler->GetWindow()->setVerticalSyncEnabled(pass->get_bool(1));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockSystemWindowVsync::get_unlocalized_name() const
{
	return "vin_system_window_vsync";
}

const char* BlockSystemWindowVsync::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemWindowVsync::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockSystemWindowVsync::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemWindowVsync::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "enable vsync"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "window"));
	{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "0"));

	return args;
}
