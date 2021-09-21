#include "BlockSystemWindowMouseY.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void execute_release(cap::mod::block::pass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->get_real(0));
	//pass->get_real(1) = sf::Mouse::getPosition(*handler->GetWindow()).y;
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockSystemWindowMouseY::get_unlocalized_name() const
{
	return "vin_system_window_mouse_y";
}

const char* BlockSystemWindowMouseY::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemWindowMouseY::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockSystemWindowMouseY::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemWindowMouseY::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "mouse y of"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "window"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "for"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable"));

	return args;
}
