#include "BlockTest.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	std::string* message = (std::string*)(pass->GetData(0));
	pass->LogInfo(*message);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	pass->LogDebug("logging to the console");
	ExecuteRelease(pass);
}

const char* BlockTest::GetUnlocalizedName() const
{
	return "vin_print";
}

blockExecution BlockTest::PullExecuteDebug()
{
	return ExecuteDebug;
}

blockExecution BlockTest::PullExecuteRelease()
{
	return ExecuteRelease;
}

const char* BlockTest::GetCategory() const
{
	return CATEGORY_MAIN;
}

const ModBlockFlags BlockTest::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const ModBlockActions BlockTest::RuntimePreInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once; used for global containers"));

	return actions;
}

const ModBlockActions BlockTest::RuntimeInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once for each stack; used for local containers"));

	return actions;
}

const ModBlockActions BlockTest::RuntimePostInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should run for every block that exists during post init; used for local containers"));

	return actions;
}

const std::vector<BlockArgumentInitializer> BlockTest::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "example text"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "Hello, World!"));

	return args;
}
