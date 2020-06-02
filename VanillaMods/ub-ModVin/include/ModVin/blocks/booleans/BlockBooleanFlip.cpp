#include "BlockBooleanFlip.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetBool(0) = !pass->GetBool(0);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockBooleanFlip::GetUnlocalizedName() const
{
	return "vin_boolean_flip";
}

const char* BlockBooleanFlip::GetCategory() const
{
	return CATEGORY_BOOLEANS;
}

blockExecution BlockBooleanFlip::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockBooleanFlip::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockBooleanFlip::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "flip"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
