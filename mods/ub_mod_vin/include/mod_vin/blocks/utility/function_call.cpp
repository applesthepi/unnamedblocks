#include "BlockUtilityFunctionCall.hpp"
#include "BlockUtilityFunctionDefine.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->AddCallstack(*(uint64_t*)(pass->GetPreData(0)), 0);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	FunctionFinder* finder = (FunctionFinder*)preData.GetStructure(FUNCTION_FINDER_NAME);
	uint64_t* loc = new uint64_t;
	std::string* str = (std::string*)(blockData.GetData()[0]);
	
	*loc = finder->GetFunctionStackIdx(*str);
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
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::RAW, "function"));

	return args;
}
