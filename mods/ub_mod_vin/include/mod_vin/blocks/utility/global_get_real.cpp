#include "BlockUtilityGlobalGetReal.hpp"
#include "aid/UtilityGlobalReal.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	UtilityGlobalReal* global = (UtilityGlobalReal*)pass->CustomGet(*(u64*)pass->GetPreData(0));
	pass->get_real(1) = global->Get(*(u64*)pass->GetPreData(1));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
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

const char* BlockUtilityGlobalGetReal::get_unlocalized_name() const
{
	return "vin_utility_global_get_real";
}

const char* BlockUtilityGlobalGetReal::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityGlobalGetReal::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityGlobalGetReal::pull_execute_release() const
{
	return execute_release;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityGlobalGetReal::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;

	stages.push_back(std::make_pair(RuntimeInit0, 0));
	stages.push_back(std::make_pair(RuntimeInit1, 1));

	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityGlobalGetReal::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "global get"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::RAW, "global"));
	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "for"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable"));

	return args;
}
