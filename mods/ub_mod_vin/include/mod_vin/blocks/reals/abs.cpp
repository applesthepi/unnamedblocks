#include "BlockRealAbs.hpp"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = std::abs(pass->GetReal(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealAbs::get_unlocalized_name() const
{
	return "vin_real_abs";
}

const char* BlockRealAbs::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealAbs::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealAbs::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealAbs::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "abs"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}