#include "BlockRealRandom.h"
#include <cmath>
#include <time.h>

static bool RuntimeGlobalPreInit(PreProcessorData& data)
{
	srand(time(NULL));
	return true;
}

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetReal(0) = (double)((double)std::rand() / RAND_MAX);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockRealRandom::GetUnlocalizedName() const
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

blockInitialization BlockRealRandom::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

const std::vector<BlockArgumentInitializer> BlockRealRandom::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to random"));

	return args;
}