#include "BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{	
	sf::RenderWindow* window = new sf::RenderWindow();

	window->create(sf::VideoMode(1920, 1080), "Unnamed Blocks Runtime Environment");
	window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(true);

	window->clear(sf::Color(30, 30, 40));
	window->display();

	WindowHandler* handler = new WindowHandler(pass, window);
	pass->GetReal(0) = pass->CustomPut(handler);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeGlobalPreinit(PreProcessorData&)
{
	InputHandler::Initialization();
	return true;
}

const char* BlockSystemCreateWindow::GetUnlocalizedName() const
{
	return "vin_system_create_window";
}

const char* BlockSystemCreateWindow::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemCreateWindow::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemCreateWindow::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockInitialization BlockSystemCreateWindow::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreinit;
}

const std::vector<BlockArgumentInitializer> BlockSystemCreateWindow::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "create window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
