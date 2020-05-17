#include "BlockRealEulers.h"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = std::exp(1.0);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealEulers::GetUnlocalizedName() const
{
	return "vin_real_eulers";
}

const char* BlockRealEulers::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealEulers::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealEulers::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealEulers::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to Euler"));

	return args;
}