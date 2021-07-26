#include "BlockUtilityGlobalGetReal.hpp"
#include "aid/UtilityGlobalReal.hpp"

#include <Cappuccino/Registration.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	UtilityGlobalReal* global = (UtilityGlobalReal*)pass->CustomGet(*(u64*)pass->GetPreData(0));
	pass->GetReal(1) = global->Get(*(u64*)pass->GetPreData(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeInit0(PreProcessorData& preData, ModBlockData& blockData)
{
	blockData.GetPreData().push_back(preData.GetStructure(GLOBAL_REAL_CUSTOM));
	UtilityGlobalReal* global = (UtilityGlobalReal*)preData.GetStructure(GLOBAL_REAL_NAME);

	global->Put(*(std::string*)blockData.GetData()[0]);

	return true;
}

static bool RuntimeInit1(PreProcessorData& preData, ModBlockData& blockData)
{
	u64* idx = new u64;
	*idx = ((UtilityGlobalReal*)preData.GetStructure(GLOBAL_REAL_NAME))->GetIdx(*(std::string*)blockData.GetData()[0]);

	blockData.GetPreData().push_back(idx);

	return true;
}

const char* BlockUtilityGlobalGetReal::GetUnlocalizedName() const
{
	return "vin_utility_global_get_real";
}

const char* BlockUtilityGlobalGetReal::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityGlobalGetReal::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityGlobalGetReal::PullExecuteRelease() const
{
	return ExecuteRelease;
}

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityGlobalGetReal::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;

	stages.push_back(std::make_pair(RuntimeInit0, 0));
	stages.push_back(std::make_pair(RuntimeInit1, 1));

	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityGlobalGetReal::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "global get"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::RAW, "global"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
