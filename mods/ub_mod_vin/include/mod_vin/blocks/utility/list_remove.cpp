#include "BlockUtilityListRemove.hpp"

#include <Cappuccino/Registration.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->GetReal(0));
	list->erase(list->begin() + pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->GetReal(0));

	if (pass->GetReal(1) >= list->size() || pass->GetReal(1) < 0 || std::floor(pass->GetReal(1)) != pass->GetReal(1))
	{
		pass->LogError("failed to remove index \"" + std::to_string(pass->GetReal(1)) + "\" of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	list->erase(list->begin() + pass->GetReal(1));
}

const char* BlockUtilityListRemove::get_unlocalized_name() const
{
	return "vin_utility_list_remove";
}

const char* BlockUtilityListRemove::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListRemove::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListRemove::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListRemove::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "remove"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "copy"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}
