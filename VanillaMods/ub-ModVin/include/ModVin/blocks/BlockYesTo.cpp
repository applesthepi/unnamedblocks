#include "BlockYesTo.h"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetCallstackBlock().back() = pass->GetVariableReal(*pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeLocalPreInit(PreProcessorData& data)
{
	data.AddStructure("yes", new YesFinder());
	return true;
}

static bool RuntimeLocalPostInit(PreProcessorData& data)
{
	delete data.GetStructure("yes");
	return true;
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	YesFinder* finder = (YesFinder*)preData.GetStructure("yes");
	int64_t idx = finder->FindPoint(*(std::string*)blockData.GetCData()[0]);

	if (idx == -1)
		return false;

	preData.GetReal(*(uint64_t*)blockData.GetCData()[1]) = (double)idx;

	return true;
}

static bool RuntimeGlobalPreInit(PreProcessorData& data)
{
	Logger::Info("from yesto");
	return true;
}

const char* BlockYesTo::GetUnlocalizedName() const
{
	return "vin_yes_to";
}

blockExecution BlockYesTo::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockYesTo::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const char* BlockYesTo::GetCategory() const
{
	return CATEGORY_OTHER;
}

const ModBlockFlags BlockYesTo::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

blockInitialization BlockYesTo::GetRuntimeGlobalPreInit() const
{
	return RuntimeGlobalPreInit;
}

blockInitialization BlockYesTo::GetRuntimeLocalPreInit() const
{
	return RuntimeLocalPreInit;
}

blockInitialization BlockYesTo::GetRuntimeLocalPostInit() const
{
	return RuntimeLocalPostInit;
}

blockDataInitialization BlockYesTo::GetRuntimeInit() const
{
	return RuntimeInit;
}

const std::vector<BlockArgumentInitializer> BlockYesTo::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "goto"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "yes"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "yes_loc"));

	return args;
}
