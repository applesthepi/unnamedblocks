#include "BlockUtilityListCreate.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = pass->CustomPut(new std::vector<void*>());
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockUtilityListCreate::GetUnlocalizedName() const
{
	return "vin_utility_list_create";
}

const char* BlockUtilityListCreate::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListCreate::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListCreate::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListCreate::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list create"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "list"));

	return args;
}
