#include "BlockUtilityListInsert.hpp"
#include "aid/UtilityList.hpp"

#include <Cappuccino/Registration.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	if (pass->GetBool(3))
		((UtilityList*)pass->CustomGet(pass->GetReal(0)))->InsertValue(new double(pass->GetReal(1)), pass->GetReal(2), true);
	else
		((UtilityList*)pass->CustomGet(pass->GetReal(0)))->InsertValue(&pass->GetReal(1), pass->GetReal(2), false);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	UtilityList* list = (UtilityList*)pass->CustomGet(pass->GetReal(0));

	if (pass->GetReal(2) >= list->Size() || pass->GetReal(2) < 0 || std::floor(pass->GetReal(2)) != pass->GetReal(2))
	{
		pass->LogError("failed to insert index \"" + std::to_string(pass->GetReal(1)) + "\" of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	if (pass->GetBool(3))
		list->InsertValue(new double(pass->GetReal(1)), pass->GetReal(2), true);
	else
		list->InsertValue(&pass->GetReal(1), pass->GetReal(2), false);
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
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "insert"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::ANY, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "copy"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}
