#include "BlockUtilityThreadKill.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	ExecutionThread* thr = (ExecutionThread*)pass->CustomGet(pass->GetReal(0));
	thr->End();
	
	Registration::UnRegisterPass(thr->GetPass());
	Registration::UnRegisterExecutionThread(thr, false);

	pass->CustomFree(pass->GetReal(0), false);
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockUtilityThreadKill::GetUnlocalizedName() const
{
	return "vin_utility_thread_kill";
}

const char* BlockUtilityThreadKill::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityThreadKill::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityThreadKill::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockUtilityThreadKill::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "kill thread"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "thread"));

	return args;
}
