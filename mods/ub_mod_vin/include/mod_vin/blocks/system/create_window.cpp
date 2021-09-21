#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>

static void execute_release(cap::mod::block::pass* pass)
{	
	//sf::RenderWindow* window = new sf::RenderWindow();

	//window->create(sf::VideoMode(1920, 1080), "Unnamed Blocks Runtime Environment");
	//window->setFramerateLimit(60);
	//window->setVerticalSyncEnabled(true);

	//window->clear(sf::Color(30, 30, 40));
	//window->display();

	//WindowHandler* handler = new WindowHandler(pass, window);
	//pass->get_real(0) = pass->CustomPut(handler);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

static bool RuntimeGlobalPreinit(PreProcessorData&)
{
	//InputHandler::Initialization();
	return true;
}

const char* BlockSystemCreateWindow::get_unlocalized_name() const
{
	return "vin_system_create_window";
}

const char* BlockSystemCreateWindow::get_category() const
{
	return CATEGORY_SYSTEM;
}

cap::mod::block::block::execution BlockSystemCreateWindow::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockSystemCreateWindow::pull_execute_release() const
{
	return execute_release;
}

blockInitialization BlockSystemCreateWindow::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreinit;
}

std::vector<cap::mod::block::block::argument::initializer> BlockSystemCreateWindow::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "create window"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "window"));

	return args;
}
