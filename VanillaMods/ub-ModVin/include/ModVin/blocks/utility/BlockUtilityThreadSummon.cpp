#include "BlockUtilityThreadSummon.h"
#include "BlockUtilityFunctionDefine.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	ModBlockPassInitializer init;

	init.DataSize = 0;
	init.Data = nullptr;
	init.VariablesReal = pass->GetVariableRegistryReal();
	init.VariablesBool = pass->GetVariableRegistryBool();
	init.VariablesString = pass->GetVariableRegistryString();
	init.CustomRegisterMutex = pass->GetCustomRegisterMutex();
	init.CustomRegister = pass->GetCustomRegister();
	init.Stop = &Registration::Stop;
	init.VariableRegistry = pass->GetVariableRegistry();
	init.DebugMode = false;
	init.BeginTime = pass->GetBeginTime();

	ModBlockPass* np = new ModBlockPass(init);
	Registration::RegisterPass(np);

	ExecutionThread* thr = new ExecutionThread(*(uint64_t*)(pass->GetPreData(0)), Registration::GetFunctionCallCount(), Registration::GetCalls(), np);
	Registration::RegisterExecutionThread(thr);

	uint64_t putIdx = pass->CustomPut(thr);
	*pass->GetReal(0) = putIdx;

	std::function<void(ModBlockPass*)>* dealloc = new std::function<void(ModBlockPass*)>();
	*dealloc = [putIdx](ModBlockPass* pass)
	{
		pass->CustomFree(putIdx, false);
	};

	pass->AddDeallocation(dealloc);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ModBlockPassInitializer init;

	init.DataSize = 0;
	init.Data = nullptr;
	init.VariablesReal = pass->GetVariableRegistryReal();
	init.VariablesBool = pass->GetVariableRegistryBool();
	init.VariablesString = pass->GetVariableRegistryString();
	init.CustomRegisterMutex = pass->GetCustomRegisterMutex();
	init.CustomRegister = pass->GetCustomRegister();
	init.Stop = &Registration::Stop;
	init.VariableRegistry = pass->GetVariableRegistry();
	init.DebugMode = true;
	init.BeginTime = pass->GetBeginTime();

	ModBlockPass* np = new ModBlockPass(init);
	Registration::RegisterPass(np);

	ExecutionThread* thr = new ExecutionThread(*(uint64_t*)(pass->GetPreData(0)), Registration::GetFunctionCallCount(), Registration::GetCalls(), np);
	Registration::RegisterExecutionThread(thr);

	uint64_t putIdx = pass->CustomPut(thr);
	*pass->GetReal(0) = putIdx;

	std::function<void(ModBlockPass*)>* dealloc = new std::function<void(ModBlockPass*)>();
	*dealloc = [putIdx](ModBlockPass* pass)
	{
		pass->CustomFree(putIdx, false);
	};

	pass->AddDeallocation(dealloc);
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	FunctionFinder* finder = (FunctionFinder*)preData.GetStructure(FUNCTION_FINDER_NAME);
	uint64_t* loc = new uint64_t;

	*loc = finder->GetFunctionStackIdx(*(std::string*)blockData.GetData()[0]);
	blockData.GetPreData().push_back(loc);

	return true;
}

const char* BlockUtilityThreadSummon::GetUnlocalizedName() const
{
	return "vin_utility_thread_summon";
}

const char* BlockUtilityThreadSummon::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityThreadSummon::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityThreadSummon::PullExecuteRelease() const
{
	return ExecuteRelease;
}

std::vector<std::pair<blockDataInitialization, uint16_t>> BlockUtilityThreadSummon::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, uint16_t>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 1));
	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityThreadSummon::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "summon thread"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "thread"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "function"));

	return args;
}