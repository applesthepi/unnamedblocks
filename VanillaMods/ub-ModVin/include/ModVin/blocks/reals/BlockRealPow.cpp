#include "BlockRealPow.h"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = std::pow(*pass->GetReal(0), *pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealPow::GetUnlocalizedName() const
{
	return "vin_real_pow";
}

const char* BlockRealPow::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealPow::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealPow::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealPow::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to power"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}