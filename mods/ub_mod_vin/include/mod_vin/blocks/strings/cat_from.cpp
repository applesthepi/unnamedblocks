#include "BlockStringCatFrom.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->get_string(0) += pass->get_string(1);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	execute_release(pass);
}

const char* BlockStringCatFrom::get_unlocalized_name() const
{
	return "vin_string_cat_from";
}

const char* BlockStringCatFrom::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringCatFrom::PullExecuteRelease() const
{
	return execute_release;
}

blockExecution BlockStringCatFrom::PullExecuteDebug() const
{
	return execute_debug;
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
