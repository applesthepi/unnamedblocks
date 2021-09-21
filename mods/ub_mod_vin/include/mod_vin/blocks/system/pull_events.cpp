#include "BlockSystemPullEvents.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void execute_release(cap::mod::block::pass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->get_real(0));
	//sf::RenderWindow* window = handler->GetWindow();
	//
	//sf::Event ev;

	//while (window->pollEvent(ev))
	//{
	//	if (ev.type == sf::Event::Closed)
	//	{
	//		window->close();
	//		pass->Stop();
	//		return;
	//	}
	//	else if (ev.type == sf::Event::MouseWheelScrolled)
	//	{
	//		handler->AddScroll(ev.mouseWheelScroll.delta);
	//	}
	//}
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockSystemPullEvents::get_unlocalized_name() const
{
	return "vin_system_pull_events";
}

const char* BlockSystemPullEvents::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemPullEvents::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockSystemPullEvents::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemPullEvents::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "window pull events"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "window"));

	return args;
}
