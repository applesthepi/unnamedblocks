#include "BlockUtilityListDestroy.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->CustomFree(pass->GetReal(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockUtilityListDestroy::GetUnlocalizedName() const
{
	return "vin_utility_list_destroy";
}

const char* BlockUtilityListDestroy::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListDestroy::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListDestroy::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListDestroy::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list destroy"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "list"));

	return args;
}
