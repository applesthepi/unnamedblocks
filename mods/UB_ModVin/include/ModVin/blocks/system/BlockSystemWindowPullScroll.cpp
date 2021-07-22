#include "BlockSystemWindowPullScroll.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(0));
	//pass->GetReal(1) = handler->PullScroll();
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemWindowPullScroll::GetUnlocalizedName() const
{
	return "vin_system_window_pull_scroll";
}

const char* BlockSystemWindowPullScroll::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemWindowPullScroll::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemWindowPullScroll::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemWindowPullScroll::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "window pull scroll"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
