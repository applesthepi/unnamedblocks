#include "BlockUtilityMarkPoint.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>

static void execute_release(cap::mod::block::pass* pass)
{

}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

static bool RuntimeLocalPreInit(PreProcessorData& data)
{
	data.AddStructure(POINT_FINDER_NAME + std::to_string(data.StackIdx), new PointFinder());
	return true;
}

static bool RuntimeLocalPostInit(PreProcessorData& data)
{
	delete data.GetStructure(POINT_FINDER_NAME + std::to_string(data.StackIdx));
	return true;
}

static bool RuntimeInit(PreProcessorData& preData, cap::mod::block::data& blockData)
{
	PointFinder* finder = (PointFinder*)preData.GetStructure(POINT_FINDER_NAME + std::to_string(preData.StackIdx));
	std::string* name = (std::string*)blockData.GetData()[0];
	finder->AddPoint(preData.BlockIdx, *name);

	return true;
}

const char* BlockUtilityMarkPoint::get_unlocalized_name() const
{
	return "vin_utility_mark_point";
}

const char* BlockUtilityMarkPoint::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityMarkPoint::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityMarkPoint::pull_execute_release() const
{
	return execute_release;
}

blockInitialization BlockUtilityMarkPoint::GetRuntimeLocalPreInit() const
{
	return RuntimeLocalPreInit;
}

blockInitialization BlockUtilityMarkPoint::GetRuntimeLocalPostInit() const
{
	return RuntimeLocalPostInit;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityMarkPoint::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 0));
	return stages;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityMarkPoint::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "mark point"));
	{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "point"));

	return args;
}
