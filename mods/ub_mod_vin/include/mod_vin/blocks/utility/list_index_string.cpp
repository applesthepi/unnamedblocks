#include "BlockUtilityListIndexString.hpp"

#include <Cappuccino/Registration.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(2) = *(std::string*)((std::vector<void*>*)pass->CustomGet(pass->GetReal(0)))->at(pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->GetReal(0));

	if (pass->GetReal(1) >= list->size() || pass->GetReal(1) < 0 || std::floor(pass->GetReal(1)) != pass->GetReal(1))
	{
		pass->LogError("failed to index *string* with \"" + std::to_string(pass->GetReal(1)) + "\" of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	pass->GetString(2) = *(std::string*)list->at(pass->GetReal(1));
}

const char* BlockUtilityListIndexString::get_unlocalized_name() const
{
	return "vin_utility_list_index_string";
}

const char* BlockUtilityListIndexString::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListIndexString::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListIndexString::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListIndexString::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "index"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
