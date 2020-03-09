#include "ModBlock.h"

const char* ModBlock::UsePath()
{
	return "res/mods/default/block.txt";
}

bool ModBlock::IsTopical()
{
	return false;
}

ModBlockFlags ModBlock::GetFlags()
{
	return ModBlockFlags();
}

ModBlockActions ModBlock::RuntimePreInit()
{
	return ModBlockActions();
};

std::vector<BlockArgumentInitializer> ModBlock::GetArguments()
{
	return {BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL")};
};
