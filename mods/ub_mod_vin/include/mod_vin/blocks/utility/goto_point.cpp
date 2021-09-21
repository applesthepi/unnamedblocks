#include "BlockUtilityGotoPoint.hpp"
#include "BlockUtilityMarkPoint.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>

static void execute_release(cap::mod::block::pass* pass)
{
	pass->SetBlockIdx(*(i64*)pass->GetPreData(0));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

static bool RuntimeInit(PreProcessorData& preData, cap::mod::block::data& blockData)
{
	PointFinder* finder = (PointFinder*)preData.GetStructure(POINT_FINDER_NAME + std::to_string(preData.StackIdx));
	i64 idx = finder->FindPoint(*(std::string*)blockData.GetData()[0]);

	if (idx == -1)
		return false;
	
	//*(f64*)blockData.GetCData()[1] = idx;

	i64* hIdx = new i64;
	*hIdx = idx;

	blockData.GetPreData().push_back(hIdx);

	return true;
}

const char* BlockUtilityGotoPoint::get_unlocalized_name() const
{
	return "vin_utility_goto_point";
}

const char* BlockUtilityGotoPoint::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityGotoPoint::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityGotoPoint::pull_execute_release() const
{
	return execute_release;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityGotoPoint::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 0));
	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityGotoPoint::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "goto"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "point"));
	//{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "idx"));

	return args;
}
