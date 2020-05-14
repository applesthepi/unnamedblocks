#include "BlockRealCos.h"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = std::cos(*pass->GetReal(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealCos::GetUnlocalizedName() const
{
	return "vin_real_cos";
}

const char* BlockRealCos::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealCos::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealCos::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealCos::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "cos"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}