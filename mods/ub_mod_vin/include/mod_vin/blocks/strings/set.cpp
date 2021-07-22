#include "BlockStringSet.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(0) = pass->GetString(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockStringSet::GetUnlocalizedName() const
{
	return "vin_string_set";
}

const char* BlockStringSet::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringSet::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringSet::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringSet::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ""));

	return args;
}
