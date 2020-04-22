#include "BlockPrint.h"
#include "ModVin/Mod.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	std::string* message = (std::string*)(pass->GetData()[0]);
	Logger::Info(*message);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::string* message = (std::string*)(pass->GetData()[0]);
	pass->LogInfo("printing to console: \"" + *message);
	Logger::Info(*message);
}

const char* SomeBlock::GetUnlocalizedName() const
{
	return "test_print";
}

blockExecution SomeBlock::PullExecuteDebug()
{
	return ExecuteDebug;
}

blockExecution SomeBlock::PullExecuteRelease()
{
	return ExecuteRelease;
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
