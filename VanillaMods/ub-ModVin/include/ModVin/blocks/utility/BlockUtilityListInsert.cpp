#include "BlockUtilityListInsert.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(*pass->GetReal(0));
	list->insert(list->begin() + *pass->GetReal(2), pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(*pass->GetReal(0));

	if (*pass->GetReal(2) >= list->size() || *pass->GetReal(2) < 0 || std::floor(*pass->GetReal(2)) != *pass->GetReal(2))
	{
		pass->LogError("failed to insert index \"" + std::to_string(*pass->GetReal(1)) + "\" of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	list->insert(list->begin() + *pass->GetReal(2), pass->GetReal(1));
}

const char* BlockUtilityListInsert::GetUnlocalizedName() const
{
	return "vin_utility_list_insert";
}

const char* BlockUtilityListInsert::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListInsert::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListInsert::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListInsert::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "insert"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::ANY, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));

	return args;
}
