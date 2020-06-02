#include "BlockStringFromReal.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(0) = std::to_string(pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockStringFromReal::GetUnlocalizedName() const
{
	return "vin_string_from_real";
}

const char* BlockStringFromReal::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringFromReal::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringFromReal::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringFromReal::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
