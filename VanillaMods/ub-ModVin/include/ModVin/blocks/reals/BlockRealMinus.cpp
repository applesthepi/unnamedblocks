#include "BlockRealMinus.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) -= pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealMinus::GetUnlocalizedName() const
{
	return "vin_real_minus";
}

const char* BlockRealMinus::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealMinus::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealMinus::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealMinus::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "-="));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}