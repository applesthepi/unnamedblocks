#include "BlockRealLog.hpp"
#include <string>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->LogInfo(std::to_string(pass->GetReal(0)));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealLog::get_unlocalized_name() const
{
	return "vin_real_log";
}

const char* BlockRealLog::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealLog::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealLog::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealLog::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}