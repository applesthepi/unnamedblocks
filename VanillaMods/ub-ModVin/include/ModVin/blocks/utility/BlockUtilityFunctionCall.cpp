#include "BlockUtilityFunctionCall.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetCallstackStack().push_back(*pass->GetReal(0));
	pass->GetCallstackBlock().push_back(0);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockUtilityFunctionCall::GetUnlocalizedName() const
{
	return "vin_utility_function_call";
}

const char* BlockUtilityFunctionCall::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityFunctionCall::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityFunctionCall::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityFunctionCall::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "call function"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "function"));

	return args;
}
