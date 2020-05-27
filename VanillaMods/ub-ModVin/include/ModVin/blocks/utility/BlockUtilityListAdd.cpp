#include "BlockUtilityListAdd.h"
#include "aid/UtilityList.h"

#include <Cappuccino/Registration.h>
#include <vector>

static void ExecuteRelease(ModBlockPass* pass)
{
	if (pass->GetBool(2))
		((UtilityList*)pass->CustomGet(pass->GetReal(0)))->AddValue(new double(pass->GetReal(1)), true);
	else
		((UtilityList*)pass->CustomGet(pass->GetReal(0)))->AddValue(&pass->GetReal(1), false);
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
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "copy"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}
