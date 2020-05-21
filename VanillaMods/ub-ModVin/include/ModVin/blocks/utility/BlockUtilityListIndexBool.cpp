#include "BlockUtilityListIndexBool.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	*pass->GetBool(2) = *(bool*)((std::vector<void*>*)pass->CustomGet(*pass->GetReal(0)))->at(*pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(*pass->GetReal(0));

	if (*pass->GetReal(1) >= list->size() || *pass->GetReal(1) < 0 || std::floor(*pass->GetReal(1)) != *pass->GetReal(1))
	{
		pass->LogError("failed to index *bool* with \"" + std::to_string(*pass->GetReal(1)) + "\" of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	*pass->GetBool(2) = *(bool*)list->at(*pass->GetReal(1));
}

const char* BlockUtilityListIndexBool::GetUnlocalizedName() const
{
	return "vin_utility_list_index_bool";
}

const char* BlockUtilityListIndexBool::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListIndexBool::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListIndexBool::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListIndexBool::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "index"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
