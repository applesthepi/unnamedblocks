#include "BlockSystemWindowMouseX.h"
#include "BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(0));
	pass->GetReal(1) = sf::Mouse::getPosition(*handler->GetWindow()).x;
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowMouseX::GetUnlocalizedName() const
{
	return "vin_system_window_mouse_x";
}

const char* BlockSystemWindowMouseX::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowMouseX::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowMouseX::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowMouseX::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse x of"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
