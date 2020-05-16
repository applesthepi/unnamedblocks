#include "BlockRealDivide.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) /= *pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealDivide::GetUnlocalizedName() const
{
	return "vin_real_divide";
}

const char* BlockRealDivide::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealDivide::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealDivide::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealDivide::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "/="));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}