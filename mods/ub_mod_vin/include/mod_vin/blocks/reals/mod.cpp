#include "BlockRealMod.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = (i64)pass->GetReal(0) % (i64)pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealMod::get_unlocalized_name() const
{
	return "vin_real_mod";
}

const char* BlockRealMod::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealMod::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealMod::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealMod::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "%="));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}