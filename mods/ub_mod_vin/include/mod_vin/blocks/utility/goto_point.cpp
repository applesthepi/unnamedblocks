#include "BlockUtilityGotoPoint.hpp"
#include "BlockUtilityMarkPoint.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->SetBlockIdx(*(i64*)pass->GetPreData(0));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
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

const char* BlockUtilityGotoPoint::GetUnlocalizedName() const
{
	return "vin_utility_goto_point";
}

const char* BlockUtilityGotoPoint::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityGotoPoint::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityGotoPoint::PullExecuteRelease() const
{
	return ExecuteRelease;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityGotoPoint::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;
	stages.push_back(std::make_pair(RuntimeInit, 0));
	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityGotoPoint::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "goto"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"));
	//args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "idx"));

	return args;
}
