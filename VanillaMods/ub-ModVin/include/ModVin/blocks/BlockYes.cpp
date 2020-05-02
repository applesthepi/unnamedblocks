#include "BlockYes.h"
#include "BlockYesTo.h"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{

}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

static bool RuntimeInit(PreProcessorData& preData, ModBlockData& blockData)
{
	YesFinder* finder = (YesFinder*)preData.GetStructure("yes");
	std::string* name = (std::string*)blockData.GetCData()[0];
	finder->AddPoint(preData.BlockIdx, *name);
	return true;
}

const char* BlockYes::GetUnlocalizedName() const
{
	return "vin_yes";
}

blockExecution BlockYes::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockYes::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const char* BlockYes::GetCategory() const
{
	return CATEGORY_OTHER;
}

const ModBlockFlags BlockYes::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

blockDataInitialization BlockYes::GetRuntimeInit() const
{
	return RuntimeInit;
}

const std::vector<BlockArgumentInitializer> BlockYes::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "yes"));

	return args;
}
