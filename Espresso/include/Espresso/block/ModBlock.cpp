#include "ModBlock.h"

const char* ModBlock::UsePath() const
{
	return "res/mods/default/block.txt";
}

const char* ModBlock::UseCatagory() const
{
	return "vin_objects";
}

bool ModBlock::IsTopical() const
{
	return false;
}

ModBlockFlags ModBlock::GetFlags() const
{
	return ModBlockFlags();
}

ModBlockActions ModBlock::RuntimePreInit() const
{
	return ModBlockActions();
};

std::vector<BlockArgumentInitializer> ModBlock::GetArguments() const
{
	return {BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL")};
};
