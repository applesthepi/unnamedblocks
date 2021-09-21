#include "BlockUtilityFunctionCall.hpp"
#include "BlockUtilityFunctionDefine.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->AddCallstack(*(u64*)(pass->GetPreData(0)), 0);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

static bool RuntimeInit(PreProcessorData& preData, cap::mod::block::data& blockData)
{
	FunctionFinder* finder = (FunctionFinder*)preData.GetStructure(FUNCTION_FINDER_NAME);
	u64* loc = new u64;
	std::string* str = (std::string*)(blockData.GetData()[0]);
	
	*loc = finder->GetFunctionStackIdx(*str);
	blockData.GetPreData().push_back(loc);

	return true;
}

const char* BlockUtilityFunctionCall::get_unlocalized_name() const
{
	return "vin_utility_function_call";
}

const char* BlockUtilityFunctionCall::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityFunctionCall::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityFunctionCall::pull_execute_release() const
{
	return execute_release;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityFunctionCall::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 1));
	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityFunctionCall::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "call function"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::RAW, "function"));

	return args;
}
