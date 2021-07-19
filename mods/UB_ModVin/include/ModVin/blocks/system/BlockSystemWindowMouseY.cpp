#include "BlockSystemWindowMouseY.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(0));
	//pass->GetReal(1) = sf::Mouse::getPosition(*handler->GetWindow()).y;
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowMouseY::GetUnlocalizedName() const
{
	return "vin_system_window_mouse_y";
}

const char* BlockSystemWindowMouseY::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowMouseY::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowMouseY::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowMouseY::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse y of"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
