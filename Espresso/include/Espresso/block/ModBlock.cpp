#include "ModBlock.h"

const char* ModBlock::GetUnlocalizedName() const
{
	return "vin_null";
}

const char* ModBlock::GetPath() const
{
	return "res/mods/default/block.txt";
}

const char* ModBlock::GetCategory() const
{
	return "vin_objects";
}

const bool ModBlock::IsTopical() const
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

const std::vector<BlockArgumentInitializer> ModBlock::GetArguments() const
{
	return {BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL")};
};
