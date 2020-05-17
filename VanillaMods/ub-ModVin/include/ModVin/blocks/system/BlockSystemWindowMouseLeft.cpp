#include "BlockSystemWindowMouseLeft.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetBool(0) = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowMouseLeft::GetUnlocalizedName() const
{
	return "vin_system_window_mouse_left";
}

const char* BlockSystemWindowMouseLeft::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowMouseLeft::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowMouseLeft::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowMouseLeft::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse left for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
