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
