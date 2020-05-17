#include "BlockSystemMain.h"

static void ExecuteRelease(ModBlockPass* pass)
{

}

static void ExecuteDebug(ModBlockPass* pass)
{
	
}

const char* BlockSystemMain::GetUnlocalizedName() const
{
	return "vin_main";
}

const char* BlockSystemMain::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

bool BlockSystemMain::IsTopical() const
{
	return true;
}

blockExecution BlockSystemMain::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockSystemMain::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockSystemMain::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "main"));

	return args;
}
