#include "BlockRealForce.h"

static void ExecuteRelease(ModBlockPass* pass)
{

}

static void ExecuteDebug(ModBlockPass* pass)
{

}

const char* BlockRealForce::GetUnlocalizedName() const
{
	return "vin_real_force";
}

const char* BlockRealForce::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealForce::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealForce::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealForce::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "force"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}