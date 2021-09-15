#include "BlockStringFromRealInt.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(0) = std::to_string((i64)pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockStringFromRealInt::get_unlocalized_name() const
{
	return "vin_string_from_real_int";
}

const char* BlockStringFromRealInt::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringFromRealInt::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringFromRealInt::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringFromRealInt::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to int"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
