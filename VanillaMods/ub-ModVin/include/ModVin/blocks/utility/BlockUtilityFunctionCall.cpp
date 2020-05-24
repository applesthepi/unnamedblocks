#include "BlockUtilityFunctionCall.h"
#include "BlockUtilityFunctionDefine.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetCallstackStack().push_back(*(uint64_t*)(pass->GetPreData(0)));
	pass->GetCallstackBlock().push_back(0);
	pass->UpdateLocalCallstack();
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	FunctionFinder* finder = (FunctionFinder*)preData.GetStructure(FUNCTION_FINDER_NAME);
	uint64_t* loc = new uint64_t;

	*loc = finder->GetFunctionStackIdx(*(std::string*)blockData.GetData()[0]);
	blockData.GetPreData().push_back(loc);

	return true;
}

const char* BlockUtilityFunctionCall::GetUnlocalizedName() const
{
	return "vin_utility_function_call";
}

const char* BlockUtilityFunctionCall::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityFunctionCall::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityFunctionCall::PullExecuteRelease() const
{
	return ExecuteRelease;
}

std::vector<std::pair<blockDataInitialization, uint16_t>> BlockUtilityFunctionCall::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, uint16_t>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 1));
	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityFunctionCall::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "call function"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "function"));

	return args;
}