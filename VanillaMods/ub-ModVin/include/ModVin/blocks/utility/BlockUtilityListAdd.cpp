#include "BlockUtilityListAdd.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	((std::vector<void*>*)pass->CustomGet(*pass->GetReal(0)))->push_back(pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockUtilityListAdd::GetUnlocalizedName() const
{
	return "vin_utility_list_add";
}

const char* BlockUtilityListAdd::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListAdd::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListAdd::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListAdd::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "add"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::ANY, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
