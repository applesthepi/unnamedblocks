#include "BlockSystemWindowRender.h"
#include "BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{	
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(0));

	handler->GetWindow()->clear(sf::Color(30, 30, 40));
	handler->DrawAll();
	handler->GetWindow()->display();
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowRender::GetUnlocalizedName() const
{
	return "vin_system_window_render";
}

const char* BlockSystemWindowRender::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowRender::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowRender::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowRender::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "render window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
