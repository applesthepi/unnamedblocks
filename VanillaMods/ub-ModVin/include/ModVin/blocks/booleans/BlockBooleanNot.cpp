#include "BlockBooleanNot.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetBool(0) = !*pass->GetBool(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockBooleanNot::GetUnlocalizedName() const
{
	return "vin_boolean_not";
}

const char* BlockBooleanNot::GetCategory() const
{
	return CATEGORY_BOOLEANS;
}

blockExecution BlockBooleanNot::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockBooleanNot::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockBooleanNot::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to not"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}
