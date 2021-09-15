#include "BlockUtilityGlobalSetReal.hpp"
#include "aid/UtilityGlobalReal.hpp"

#include <Cappuccino/Registration.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	UtilityGlobalReal* global = (UtilityGlobalReal*)pass->CustomGet(*(u64*)pass->GetPreData(0));
	global->Get(*(u64*)pass->GetPreData(1)) = pass->GetReal(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
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

std::vector<std::pair<blockDataInitialization, u16>> BlockUtilityGlobalSetReal::GetRuntimeStages() const
{
	std::vector<std::pair<blockDataInitialization, u16>> stages;

	stages.push_back(std::make_pair(RuntimeInit0, 0));
	stages.push_back(std::make_pair(RuntimeInit1, 1));

	return stages;
}

const std::vector<BlockArgumentInitializer> BlockUtilityGlobalSetReal::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "global set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::RAW, "global"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
