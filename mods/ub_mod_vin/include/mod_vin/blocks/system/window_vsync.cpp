#include "BlockSystemWindowVsync.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(0));
	//handler->GetWindow()->setVerticalSyncEnabled(pass->GetBool(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowVsync::GetUnlocalizedName() const
{
	return "vin_system_window_vsync";
}

const char* BlockSystemWindowVsync::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowVsync::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowVsync::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowVsync::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "enable vsync"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}
