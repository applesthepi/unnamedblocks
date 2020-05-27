#include "BlockUtilityListCreate.h"
#include "aid/UtilityListCopies.h"

#include <Cappuccino/Registration.h>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = pass->CustomPut(new std::vector<void*>());
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeGlobalPreInit(PreProcessorData& data)
{
	data.AddStructure("vin_list", new UtilityListCopies());
	return true;
}

const char* BlockUtilityListCreate::GetUnlocalizedName() const
{
	return "vin_utility_list_create";
}

const char* BlockUtilityListCreate::GetCategory() const
{
	return CATEGORY_UTILITY;
}

blockExecution BlockUtilityListCreate::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockUtilityListCreate::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockInitialization BlockUtilityListCreate::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

const std::vector<BlockArgumentInitializer> BlockUtilityListCreate::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "list create"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "list"));

	return args;
}
