#include "BlockSystemWindowRender.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>

static void execute_release(cap::mod::block::pass* pass)
{	
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->get_real(0));

	//handler->GetWindow()->clear(sf::Color(30, 30, 40));
	//handler->DrawAll();
	//handler->GetWindow()->display();
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockSystemWindowRender::get_unlocalized_name() const
{
	return "vin_system_window_render";
}

const char* BlockSystemWindowRender::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemWindowRender::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockSystemWindowRender::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemWindowRender::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "render window"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "window"));

	return args;
}
