#include "BlockUtilityFunctionDefine.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	
}

static void ExecuteDebug(ModBlockPass* pass)
{
	
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	FunctionFinder* finder = (FunctionFinder*)preData.GetStructure(FUNCTION_FINDER_NAME);
	finder->AddFunction(*(std::string*)blockData.GetData()[0], preData.StackIdx);

	return true;
}

static bool RuntimeGlobalPreInit(PreProcessorData& data)
{
	data.AddStructure(FUNCTION_FINDER_NAME, new FunctionFinder());
	return true;
}

static bool RuntimeGlobalPostInit(PreProcessorData& data)
{
	delete data.GetStructure(FUNCTION_FINDER_NAME);
	return true;
}

const char* BlockUtilityFunctionDefine::GetUnlocalizedName() const
{
	return "vin_utility_function_define";
}

const char* BlockUtilityFunctionDefine::GetCategory() const
{
	return CATEGORY_UTILITY;
}

bool BlockUtilityFunctionDefine::IsTopical() const
{
	return true;
}

blockExecution BlockUtilityFunctionDefine::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityFunctionDefine::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockInitialization BlockUtilityFunctionDefine::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

blockInitialization BlockUtilityFunctionDefine::GetRuntimeGlobalPostInit() const
{
	return RuntimeGlobalPostInit;
}

std::vector<std::pair<blockDataInitialization, uint16_t>> BlockUtilityFunctionDefine::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, uint16_t>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 0));
	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityFunctionDefine::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "define function"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "function"));

	return args;
}
