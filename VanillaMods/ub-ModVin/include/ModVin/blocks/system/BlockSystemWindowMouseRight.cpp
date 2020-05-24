#include "BlockSystemWindowMouseRight.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetBool(0) = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowMouseRight::GetUnlocalizedName() const
{
	return "vin_system_window_mouse_right";
}

const char* BlockSystemWindowMouseRight::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowMouseRight::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowMouseRight::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowMouseRight::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse right for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
