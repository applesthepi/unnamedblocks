#include "BlockUtilityMarkPoint.h"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{

}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
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

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	PointFinder* finder = (PointFinder*)preData.GetStructure(POINT_FINDER_NAME + std::to_string(preData.StackIdx));
	std::string* name = (std::string*)blockData.GetCData()[0];
	finder->AddPoint(preData.BlockIdx, *name);

	return true;
}

const char* BlockUtilityMarkPoint::GetUnlocalizedName() const
{
	return "vin_utility_mark_point";
}

const char* BlockUtilityMarkPoint::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityMarkPoint::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityMarkPoint::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockInitialization BlockUtilityMarkPoint::GetRuntimeLocalPreInit() const
{
	return RuntimeLocalPreInit;
}

blockInitialization BlockUtilityMarkPoint::GetRuntimeLocalPostInit() const
{
	return RuntimeLocalPostInit;
}

std::vector<std::pair<blockDataInitialization, uint16_t>> BlockUtilityMarkPoint::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, uint16_t>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 0));
	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityMarkPoint::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mark point"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"));

	return args;
}
