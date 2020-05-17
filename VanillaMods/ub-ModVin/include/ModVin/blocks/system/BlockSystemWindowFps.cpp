#include "BlockSystemWindowFps.h"
#include "BlockSystemCreateWindow.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	WindowHandler* handler = (WindowHandler*)pass->CustomGet(*pass->GetReal(0));
	handler->GetWindow()->setFramerateLimit(*pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowFps::GetUnlocalizedName() const
{
	return "vin_system_window_fps";
}

const char* BlockSystemWindowFps::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowFps::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowFps::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowFps::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "fps limit"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "250"));

	return args;
}
