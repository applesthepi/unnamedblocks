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

blockExecution ModBlock::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution ModBlock::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const char* ModBlock::GetCategory() const
{
	return "vin_null";
}

bool ModBlock::IsTopical() const
{
	return false;
}

const ModBlockFlags ModBlock::GetFlags() const
{
	return ModBlockFlags();
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

blockDataInitialization ModBlock::GetRuntimeInit() const
{
	return RuntimeDataInitialization;
}

const std::vector<BlockArgumentInitializer> ModBlock::GetArguments() const
{
	return {BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL")};
}
