#include "BlockSystemWindowMouseRight.hpp"

//#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void execute_release(cap::mod::block::pass* pass)
{
	//pass->get_bool(0) = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockSystemWindowMouseRight::get_unlocalized_name() const
{
	return "vin_system_window_mouse_right";
}

const char* BlockSystemWindowMouseRight::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemWindowMouseRight::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockSystemWindowMouseRight::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemWindowMouseRight::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "mouse right for"));
	{ cap::mod::block::block::argument::type::BOOL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable"));

	return args;
}
