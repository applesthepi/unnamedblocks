#include "BlockStringFromBool.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(0) = (pass->GetBool(1)) ? "true" : "false";
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockStringFromBool::GetUnlocalizedName() const
{
	return "vin_string_from_bool";
}

const char* BlockStringFromBool::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringFromBool::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringFromBool::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringFromBool::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
