#include "BlockBooleanLog.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->LogInfo(pass->GetBool(0) ? "true" : "false");
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockBooleanLog::GetUnlocalizedName() const
{
	return "vin_boolean_log";
}

const char* BlockBooleanLog::GetCategory() const
{
	return CATEGORY_BOOLEANS;
}

blockExecution BlockBooleanLog::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockBooleanLog::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockBooleanLog::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
