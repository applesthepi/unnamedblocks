#include "BlockRealFloor.hpp"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = std::floor(pass->GetReal(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealFloor::GetUnlocalizedName() const
{
	return "vin_real_floor";
}

const char* BlockRealFloor::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealFloor::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealFloor::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealFloor::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "floor"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}