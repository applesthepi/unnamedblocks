#include "BlockUtilityThreadSummon.h"

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

	ExecutionThread* thr = new ExecutionThread((uint64_t)*pass->GetReal(1), Registration::GetFunctionCallCount(), Registration::GetCalls(), np);
	Registration::RegisterExecutionThread(thr);

	*pass->GetReal(0) = pass->CustomPut(thr);
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

	ExecutionThread* thr = new ExecutionThread(*pass->GetReal(1), Registration::GetFunctionCallCount(), Registration::GetCalls(), np);

	*pass->GetReal(0) = pass->CustomPut(thr);
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

const std::vector<BlockArgumentInitializer> BlockUtilityThreadSummon::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "summon thread"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "thread"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "function"));

	return args;
}
