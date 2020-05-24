#include "BlockRealTan.h"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = std::tan(pass->GetReal(0) * (3.141592653589793 / 180.0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealTan::GetUnlocalizedName() const
{
	return "vin_real_tan";
}

const char* BlockRealTan::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealTan::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealTan::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealTan::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "tan"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}