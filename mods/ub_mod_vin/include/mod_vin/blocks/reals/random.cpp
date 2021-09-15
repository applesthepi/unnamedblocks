#include "BlockRealRandom.hpp"
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::uniform_real_distribution<f64> draw(0.0, 1.0);
	pass->GetReal(0) = draw(pass->GetRandomGenerator());
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealRandom::get_unlocalized_name() const
{
	return "vin_real_random";
}

const char* BlockRealRandom::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealRandom::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealRandom::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealRandom::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to random"));

	return args;
}