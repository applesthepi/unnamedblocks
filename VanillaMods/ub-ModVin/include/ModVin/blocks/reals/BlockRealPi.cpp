#include "BlockRealPi.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = 3.141592653589793;
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealPi::GetUnlocalizedName() const
{
	return "vin_real_pi";
}

const char* BlockRealPi::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealPi::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealPi::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealPi::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to PI"));

	return args;
}