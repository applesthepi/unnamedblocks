#include "ModBlock.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->LogWarning("executed default release function");
}

static void ExecuteDebug(ModBlockPass* pass)
{
	pass->LogWarning("executed default debug function");
}

static bool RuntimeInitialization(PreProcessorData&)
{
	return true;
}

static bool RuntimeDataInitialization(PreProcessorData&, ModBlockData&)
{
	return true;
}

const char* ModBlock::GetUnlocalizedName() const
{
	return "vin_null";
}

const char* ModBlock::GetCategory() const
{
	return "vin_null";
}

bool ModBlock::IsTopical() const
{
	return false;
}

blockExecution ModBlock::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution ModBlock::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockInitialization ModBlock::GetRuntimeGlobalPreInit() const
{
	return RuntimeInitialization;
}

blockInitialization ModBlock::GetRuntimeGlobalPostInit() const
{
	return RuntimeInitialization;
}

blockInitialization ModBlock::GetRuntimeLocalPreInit() const
{
	return RuntimeInitialization;
}

blockInitialization ModBlock::GetRuntimeLocalPostInit() const
{
	return RuntimeInitialization;
}

CAP_DLL std::vector<std::pair<blockDataInitialization, uint16_t>> ModBlock::GetRuntimeStages() const
{
	return std::vector<std::pair<blockDataInitialization, uint16_t>>();
}

const std::vector<BlockArgumentInitializer> ModBlock::GetArguments() const
{
	return {BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL")};
}
