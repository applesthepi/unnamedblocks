#include "BlockYes.h"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockYes::GetUnlocalizedName() const
{
	return "vin_yes";
}

blockExecution BlockYes::PullExecuteDebug()
{
	return ExecuteDebug;
}

blockExecution BlockYes::PullExecuteRelease()
{
	return ExecuteRelease;
}

const char* BlockYes::GetCategory() const
{
	return CATEGORY_MAIN;
}

const ModBlockFlags BlockYes::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const ModBlockActions BlockYes::RuntimePreInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once; used for global containers"));

	return actions;
}

const ModBlockActions BlockYes::RuntimeInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once for each stack; used for local containers"));

	return actions;
}

const ModBlockActions BlockYes::RuntimePostInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should run for every block that exists during post init; used for local containers"));

	return actions;
}

const std::vector<BlockArgumentInitializer> BlockYes::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "yes"));

	return args;
}
