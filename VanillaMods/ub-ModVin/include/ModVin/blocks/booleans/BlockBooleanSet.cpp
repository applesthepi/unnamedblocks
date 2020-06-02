#include "BlockBooleanSet.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetBool(0) = pass->GetBool(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockBooleanSet::GetUnlocalizedName() const
{
	return "vin_boolean_set";
}

const char* BlockBooleanSet::GetCategory() const
{
	return CATEGORY_BOOLEANS;
}

blockExecution BlockBooleanSet::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockBooleanSet::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockBooleanSet::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}
