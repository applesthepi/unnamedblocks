#include "BlockRealMultiply.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) *= pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealMultiply::GetUnlocalizedName() const
{
	return "vin_real_multiply";
}

const char* BlockRealMultiply::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealMultiply::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealMultiply::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealMultiply::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "*="));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}