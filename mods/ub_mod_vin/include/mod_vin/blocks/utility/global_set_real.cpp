#include "BlockUtilityGlobalSetReal.hpp"
#include "aid/UtilityGlobalReal.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	UtilityGlobalReal* global = (UtilityGlobalReal*)pass->CustomGet(*(u64*)pass->GetPreData(0));
	global->Get(*(u64*)pass->GetPreData(1)) = pass->get_real(1);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

static bool RuntimeGlobalPreInit(PreProcessorData& data)
{
	UtilityGlobalReal* global = new UtilityGlobalReal();
	u64* globalIdx = new u64;

	*globalIdx = data.CustomPut(global);

	data.AddStructure(GLOBAL_REAL_NAME, global);
	data.AddStructure(GLOBAL_REAL_CUSTOM, globalIdx);

	return true;
}

static bool RuntimeGlobalPostInit(PreProcessorData& data)
{
	UtilityGlobalReal* global = (UtilityGlobalReal*)data.GetStructure(GLOBAL_REAL_NAME);
	global->Bake();

	return true;
}

static bool RuntimeInit0(PreProcessorData& preData, cap::mod::block::data& blockData)
{
	blockData.GetPreData().push_back(preData.GetStructure(GLOBAL_REAL_CUSTOM));
	UtilityGlobalReal* global = (UtilityGlobalReal*)preData.GetStructure(GLOBAL_REAL_NAME);

	global->Put(*(std::string*)blockData.GetData()[0]);

	return true;
}

static bool RuntimeInit1(PreProcessorData& preData, cap::mod::block::data& blockData)
{
	u64* idx = new u64;
	*idx = ((UtilityGlobalReal*)preData.GetStructure(GLOBAL_REAL_NAME))->GetIdx(*(std::string*)blockData.GetData()[0]);

	blockData.GetPreData().push_back(idx);

	return true;
}

const char* BlockUtilityGlobalSetReal::get_unlocalized_name() const
{
	return "vin_utility_global_set_real";
}

const char* BlockUtilityGlobalSetReal::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityGlobalSetReal::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityGlobalSetReal::pull_execute_release() const
{
	return execute_release;
}

blockInitialization BlockUtilityGlobalSetReal::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

blockInitialization BlockUtilityGlobalSetReal::GetRuntimeGlobalPostInit() const
{
	return RuntimeGlobalPostInit;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityGlobalSetReal::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;

	stages.push_back(std::make_pair(RuntimeInit0, 0));
	stages.push_back(std::make_pair(RuntimeInit1, 1));

	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityGlobalSetReal::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "global set"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::RAW, "global"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "to"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable"));

	return args;
}
