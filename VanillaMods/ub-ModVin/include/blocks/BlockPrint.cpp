#include "BlockPrint.h"

const char* SomeBlock::GetUnlocalizedName() const
{
	return "test_print";
}

const char* SomeBlock::GetPath() const
{
	return "TestMod/print.ubb";
}

const char* SomeBlock::GetCategory() const
{
	return CATEGORY_MAIN;
}

const ModBlockFlags SomeBlock::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const ModBlockActions SomeBlock::RuntimePreInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once; used for global containers"));

	return actions;
}

const ModBlockActions SomeBlock::RuntimeInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once for each stack; used for local containers"));

	return actions;
}

const ModBlockActions SomeBlock::RuntimePostInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should run for every block that exists during post init; used for local containers"));

	return actions;
}

const std::vector<BlockArgumentInitializer> SomeBlock::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "example text"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "Hello, World!"));

	return args;
}
