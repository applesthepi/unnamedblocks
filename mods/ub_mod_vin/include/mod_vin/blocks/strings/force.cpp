#include "BlockStringForce.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{

}

static void ExecuteDebug(ModBlockPass* pass)
{

}

const char* BlockStringForce::get_unlocalized_name() const
{
	return "vin_string_force";
}

const char* BlockStringForce::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringForce::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringForce::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringForce::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "force"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
