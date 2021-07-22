#include "BlockBooleanForce.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{

}

static void ExecuteDebug(ModBlockPass* pass)
{
	
}

const char* BlockBooleanForce::GetUnlocalizedName() const
{
	return "vin_boolean_force";
}

const char* BlockBooleanForce::GetCategory() const
{
	return CATEGORY_BOOLEANS;
}

blockExecution BlockBooleanForce::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockBooleanForce::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockBooleanForce::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "force"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
