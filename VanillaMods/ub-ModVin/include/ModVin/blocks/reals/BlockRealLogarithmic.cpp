#include "BlockRealLogarithmic.h"
#include <string>
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = std::log(pass->GetReal(0)) / std::log(pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealLogarithmic::GetUnlocalizedName() const
{
	return "vin_real_logarithmic";
}

const char* BlockRealLogarithmic::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealLogarithmic::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealLogarithmic::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealLogarithmic::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "logarithmic"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "base"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "2"));

	return args;
}