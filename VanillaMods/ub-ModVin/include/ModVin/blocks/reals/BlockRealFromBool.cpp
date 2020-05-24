#include "BlockRealFromBool.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = pass->GetBool(1) ? 1 : 0;
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealFromBool::GetUnlocalizedName() const
{
	return "vin_real_from_bool";
}

const char* BlockRealFromBool::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealFromBool::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealFromBool::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealFromBool::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}