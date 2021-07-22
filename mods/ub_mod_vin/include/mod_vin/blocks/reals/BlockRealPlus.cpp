#include "BlockRealPlus.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) += pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealPlus::GetUnlocalizedName() const
{
	return "vin_real_plus";
}

const char* BlockRealPlus::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealPlus::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealPlus::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealPlus::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "+="));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}