#include "BlockUtilityGlobalSetReal.h"
#include "aid/UtilityGlobalReal.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	UtilityGlobalReal* global = (UtilityGlobalReal*)pass->CustomGet(*(uint64_t*)pass->GetPreData(0));
	global->Get(*(uint64_t*)pass->GetPreData(1)) = *pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeGlobalPreInit(PreProcessorData& data)
{
	UtilityGlobalReal* global = new UtilityGlobalReal();
	uint64_t* globalIdx = new uint64_t;

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

static bool RuntimeInit0(PreProcessorData& preData, ModBlockData& blockData)
{
	blockData.GetPreData().push_back(preData.GetStructure(GLOBAL_REAL_CUSTOM));
	UtilityGlobalReal* global = (UtilityGlobalReal*)preData.GetStructure(GLOBAL_REAL_NAME);

	global->Put(*(std::string*)blockData.GetData()[0]);

	return true;
}

static bool RuntimeInit1(PreProcessorData& preData, ModBlockData& blockData)
{
	uint64_t* idx = new uint64_t;
	*idx = ((UtilityGlobalReal*)preData.GetStructure(GLOBAL_REAL_NAME))->GetIdx(*(std::string*)blockData.GetData()[0]);

	blockData.GetPreData().push_back(idx);

	return true;
}

const char* BlockUtilityGlobalSetReal::GetUnlocalizedName() const
{
	return "vin_utility_global_set_real";
}

const char* BlockUtilityGlobalSetReal::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityGlobalSetReal::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityGlobalSetReal::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockInitialization BlockUtilityGlobalSetReal::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

blockInitialization BlockUtilityGlobalSetReal::GetRuntimeGlobalPostInit() const
{
	return RuntimeGlobalPostInit;
}

std::vector<std::pair<blockDataInitialization, uint16_t>> BlockUtilityGlobalSetReal::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, uint16_t>> stages;

	stages.push_back(std::make_pair(RuntimeInit0, 0));
	stages.push_back(std::make_pair(RuntimeInit1, 1));

	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityGlobalSetReal::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "global set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "global"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
