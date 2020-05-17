#include "BlockUtilityGotoPoint.h"
#include "BlockUtilityMarkPoint.h"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetCallstackBlock().back() = *(int64_t*)pass->GetPreData(0);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	PointFinder* finder = (PointFinder*)preData.GetStructure(POINT_FINDER_NAME);
	int64_t idx = finder->FindPoint(*(std::string*)blockData.GetCData()[0]);

	if (idx == -1)
		return false;
	
	//*(double*)blockData.GetCData()[1] = idx;

	int64_t* hIdx = new int64_t;
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

blockDataInitialization BlockUtilityGotoPoint::GetRuntimeInit() const
{
	return RuntimeInit;
}

const std::vector<BlockArgumentInitializer> BlockUtilityGotoPoint::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "goto"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"));
	//args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "idx"));

	return args;
}
