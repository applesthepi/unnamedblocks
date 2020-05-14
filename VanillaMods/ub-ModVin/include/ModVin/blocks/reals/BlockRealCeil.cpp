#include "BlockRealCeil.h"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = std::ceil(*pass->GetReal(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealCeil::GetUnlocalizedName() const
{
	return "vin_real_ceil";
}

const char* BlockRealCeil::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealCeil::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealCeil::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealCeil::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "ceil"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}