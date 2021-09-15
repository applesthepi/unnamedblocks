#include "BlockRealCompLsEq.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetBool(2) = pass->GetReal(0) <= pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealCompLsEq::get_unlocalized_name() const
{
	return "vin_real_comp_ls_eq";
}

const char* BlockRealCompLsEq::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealCompLsEq::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealCompLsEq::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealCompLsEq::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "<="));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}