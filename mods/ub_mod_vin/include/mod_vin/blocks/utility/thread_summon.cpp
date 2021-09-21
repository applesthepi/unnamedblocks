#include "BlockUtilityThreadSummon.hpp"
#include "BlockUtilityFunctionDefine.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	ModBlockPassInitializer init;

	init.DataSize = 0;
	init.Data = nullptr;
	init.VariablesRealCount = Registration::GetRealCount();
	init.VariablesBoolCount = Registration::GetBoolCount();
	init.VariablesStringCount = Registration::GetStringCount();
	init.CustomRegisterMutex = Registration::GetCustomMutex();
	init.CustomRegister = Registration::GetCustomRegistry();
	init.Stop = &Registration::Stop;
	init.VariableRegistry = nullptr;// TODO isnt working right now
	init.DebugMode = false;
	init.BeginTime = pass->GetBeginTime();

	cap::mod::block::pass* np = new cap::mod::block::pass(init);
	Registration::RegisterPass(np);

	ExecutionThread* thr = new ExecutionThread(*(u64*)(pass->GetPreData(0)), Registration::GetFunctionCallCount(), Registration::GetCalls(), np);
	Registration::RegisterExecutionThread(thr);

	u64 putIdx = pass->CustomPut(thr);
	pass->get_real(0) = putIdx;

	std::function<void(cap::mod::block::pass*)>* dealloc = new std::function<void(cap::mod::block::pass*)>();
	*dealloc = [putIdx](cap::mod::block::pass* pass)
	{
		pass->CustomFree(putIdx, false);
	};

	pass->AddDeallocation(dealloc);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	ModBlockPassInitializer init;

	init.DataSize = 0;
	init.Data = nullptr;
	init.VariablesRealCount = Registration::GetRealCount();
	init.VariablesBoolCount = Registration::GetBoolCount();
	init.VariablesStringCount = Registration::GetStringCount();
	init.CustomRegisterMutex = Registration::GetCustomMutex();
	init.CustomRegister = Registration::GetCustomRegistry();
	init.Stop = &Registration::Stop;
	init.VariableRegistry = nullptr;// TODO isnt working right now
	init.DebugMode = true;
	init.BeginTime = pass->GetBeginTime();

	cap::mod::block::pass* np = new cap::mod::block::pass(init);
	Registration::RegisterPass(np);

	ExecutionThread* thr = new ExecutionThread(*(u64*)(pass->GetPreData(0)), Registration::GetFunctionCallCount(), Registration::GetCalls(), np);
	Registration::RegisterExecutionThread(thr);

	u64 putIdx = pass->CustomPut(thr);
	pass->get_real(0) = putIdx;

	std::function<void(cap::mod::block::pass*)>* dealloc = new std::function<void(cap::mod::block::pass*)>();
	*dealloc = [putIdx](cap::mod::block::pass* pass)
	{
		pass->CustomFree(putIdx, false);
	};

	pass->AddDeallocation(dealloc);
}

static bool RuntimeInit(PreProcessorData& preData, cap::mod::block::data& blockData)
{
	FunctionFinder* finder = (FunctionFinder*)preData.GetStructure(FUNCTION_FINDER_NAME);
	u64* loc = new u64;
	std::string* str = (std::string*)(blockData.GetData()[1]);

	*loc = finder->GetFunctionStackIdx(*str);
	blockData.GetPreData().push_back(loc);

	return true;
}

const char* BlockUtilityThreadSummon::get_unlocalized_name() const
{
	return "vin_utility_thread_summon";
}

const char* BlockUtilityThreadSummon::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityThreadSummon::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityThreadSummon::pull_execute_release() const
{
	return execute_release;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityThreadSummon::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 1));
	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityThreadSummon::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "summon thread"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "thread"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "at"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::RAW, "function"));

	return args;
}
