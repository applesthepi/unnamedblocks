#include "BlockTest.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->LogInfo(*pass->GetString(0));
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

blockExecution BlockTest::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockTest::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const char* BlockTest::GetCategory() const
{
	return CATEGORY_OTHER;
}

const ModBlockFlags BlockTest::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const std::vector<BlockArgumentInitializer> BlockTest::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "example text"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "Hello, World!"));

	return args;
}
