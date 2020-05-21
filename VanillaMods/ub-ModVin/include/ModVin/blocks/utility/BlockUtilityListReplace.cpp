#include "BlockUtilityListReplace.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(*pass->GetReal(0));
	(*list)[*pass->GetReal(1)] = pass->GetReal(2);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(*pass->GetReal(0));

	if (*pass->GetReal(1) >= list->size() || *pass->GetReal(1) < 0 || std::floor(*pass->GetReal(1)) != *pass->GetReal(1))
	{
		pass->LogError("failed to replace index \"" + std::to_string(*pass->GetReal(1)) + "\" of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	(*list)[*pass->GetReal(1)] = pass->GetReal(2);
}

const char* BlockUtilityListReplace::GetUnlocalizedName() const
{
	return "vin_utility_list_replace";
}

const char* BlockUtilityListReplace::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListReplace::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListReplace::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListReplace::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "replace"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "with"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::ANY, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
