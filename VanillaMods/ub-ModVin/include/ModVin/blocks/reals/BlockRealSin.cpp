#include "BlockRealSin.h"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = std::sin(*pass->GetReal(0) * (3.141592653589793 / 180.0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealSin::GetUnlocalizedName() const
{
	return "vin_real_sin";
}

const char* BlockRealSin::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealSin::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealSin::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealSin::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sin"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}