#include "BlockStringCatFrom.h"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(0) += pass->GetString(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockStringCatFrom::GetUnlocalizedName() const
{
	return "vin_string_cat_from";
}

const char* BlockStringCatFrom::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringCatFrom::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringCatFrom::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringCatFrom::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "cat"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "from"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
