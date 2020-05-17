#include "BlockStringLog.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->LogInfo(*pass->GetString(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockStringLog::GetUnlocalizedName() const
{
	return "vin_string_log";
}

const char* BlockStringLog::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringLog::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringLog::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringLog::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
