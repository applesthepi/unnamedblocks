#include "BlockTestRender.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{	
	sf::RenderWindow* window = new sf::RenderWindow();

	window->create(sf::VideoMode(1920, 1080), "Unnamed Blocks Runtime Environment");
	window->setFramerateLimit(200);
	window->setVerticalSyncEnabled(false);

	window->clear(sf::Color(50, 50, 50));
	window->display();

	pass->GetVariableReal(0) = pass->CustomPut(window);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockTestRender::GetUnlocalizedName() const
{
	return "vin_render";
}

blockExecution BlockTestRender::PullExecuteDebug()
{
	return ExecuteDebug;
}

blockExecution BlockTestRender::PullExecuteRelease()
{
	return ExecuteRelease;
}

const char* BlockTestRender::GetCategory() const
{
	return CATEGORY_MAIN;
}

const ModBlockFlags BlockTestRender::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const ModBlockActions BlockTestRender::RuntimePreInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once; used for global containers"));

	return actions;
}

const ModBlockActions BlockTestRender::RuntimeInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once for each stack; used for local containers"));

	return actions;
}

const ModBlockActions BlockTestRender::RuntimePostInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should run for every block that exists during post init; used for local containers"));

	return actions;
}

const std::vector<BlockArgumentInitializer> BlockTestRender::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "create window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
