#include "BlockBool.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetBool(0) = *pass->GetBool(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockBool::GetUnlocalizedName() const
{
	return "vin_bool";
}

blockExecution BlockBool::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockBool::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const char* BlockBool::GetCategory() const
{
	return CATEGORY_OTHER;
}

const ModBlockFlags BlockBool::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const std::vector<BlockArgumentInitializer> BlockBool::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set bool"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}
