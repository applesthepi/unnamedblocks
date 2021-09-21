#include "BlockUtilityFunctionDefine.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	
}

static void execute_debug(cap::mod::block::pass* pass)
{
	
}

static bool RuntimeInit(PreProcessorData& preData, cap::mod::block::data& blockData)
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

const char* BlockUtilityFunctionDefine::get_unlocalized_name() const
{
	return "vin_utility_function_define";
}

const char* BlockUtilityFunctionDefine::get_category() const
{
	return CATEGORY_UTILITY;
}

bool BlockUtilityFunctionDefine::IsTopical() const
{
	return true;
}

cap::mod::block::block::execution BlockUtilityFunctionDefine::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityFunctionDefine::pull_execute_release() const
{
	return execute_release;
}

blockInitialization BlockUtilityFunctionDefine::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

blockInitialization BlockUtilityFunctionDefine::GetRuntimeGlobalPostInit() const
{
	return RuntimeGlobalPostInit;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityFunctionDefine::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 0));
	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityFunctionDefine::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "define function"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::RAW, "function"));

	return args;
}
