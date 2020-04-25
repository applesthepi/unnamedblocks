#include "ModBlock.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->LogInfo("blank release executed");
}

static void ExecuteDebug(ModBlockPass* pass)
{
	pass->LogInfo("blank release executed");
}

const char* ModBlock::GetUnlocalizedName() const
{
	return "vin_null";
}

blockExecution ModBlock::PullExecuteRelease()
{
	return ExecuteRelease;
}

blockExecution ModBlock::PullExecuteDebug()
{
	return ExecuteDebug;
}

const char* ModBlock::GetCategory() const
{
	return "vin_objects";
}

bool ModBlock::IsTopical() const
{
	return false;
}

const ModBlockFlags ModBlock::GetFlags() const
{
	return ModBlockFlags();
}

const ModBlockActions ModBlock::RuntimePreInit() const
{
	return ModBlockActions();
};

const ModBlockActions ModBlock::RuntimeInit() const
{
	return ModBlockActions();
};

const ModBlockActions ModBlock::RuntimePostInit() const
{
	return ModBlockActions();
}

const std::vector<BlockArgumentInitializer> ModBlock::GetArguments() const
{
	return {BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL")};
};
