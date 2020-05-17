#include "BlockMain.h"

const char* BlockMain::GetUnlocalizedName() const
{
	return "vin_main";
}

const char* BlockMain::GetCategory() const
{
	return CATEGORY_MAIN;
}

const ModBlockFlags BlockMain::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const std::vector<BlockArgumentInitializer> BlockMain::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "main"));

	return args;
}
