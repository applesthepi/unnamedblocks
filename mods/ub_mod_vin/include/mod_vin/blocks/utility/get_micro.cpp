#include "BlockUtilityGetMicro.hpp"

#include <Cappuccino/Registration.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - *pass->GetBeginTime()).count();
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockUtilityGetMicro::get_unlocalized_name() const
{
	return "vin_utility_get_micro";
}

const char* BlockUtilityGetMicro::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityGetMicro::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityGetMicro::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityGetMicro::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "get micro"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "micro"));

	return args;
}
