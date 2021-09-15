#include "BlockUtilityListReserve.hpp"

#include <Cappuccino/Registration.hpp>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->GetReal(0));
	list->reserve(pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::vector<void*>* list = (std::vector<void*>*)pass->CustomGet(pass->GetReal(0));

	if (pass->GetReal(1) >= list->size() || pass->GetReal(1) < 0 || std::floor(pass->GetReal(1)) != pass->GetReal(1))
	{
		pass->LogError("failed to reserve \"" + std::to_string(pass->GetReal(1)) + "\" capacity of a list; index is invalid", LoggerFatality::BREAK);
		return;
	}

	list->reserve(pass->GetReal(1));
}

const char* BlockUtilityListReserve::get_unlocalized_name() const
{
	return "vin_utility_list_reserve";
}

const char* BlockUtilityListReserve::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListReserve::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListReserve::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListReserve::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "list"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "reserve"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));

	return args;
}
