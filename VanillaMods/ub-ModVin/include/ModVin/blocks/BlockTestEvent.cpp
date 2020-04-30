#include "BlockTestEvent.h"

#include <SFML/Graphics.hpp>
#include <chrono>

static void ExecuteRelease(ModBlockPass* pass)
{
	sf::RenderWindow* window = (sf::RenderWindow*)pass->CustomGet(pass->GetVariableReal(0));
	sf::Event ev;

	while (true)
	{
		while (window->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				window->close();
				pass->Stop();
				return;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockTestEvent::GetUnlocalizedName() const
{
	return "vin_event";
}

blockExecution BlockTestEvent::PullExecuteDebug()
{
	return ExecuteDebug;
}

blockExecution BlockTestEvent::PullExecuteRelease()
{
	return ExecuteRelease;
}

const char* BlockTestEvent::GetCategory() const
{
	return CATEGORY_MAIN;
}

const ModBlockFlags BlockTestEvent::GetFlags() const
{
	return ModBlockFlags().CheckHeap().CheckStack();
}

const ModBlockActions BlockTestEvent::RuntimePreInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once; used for global containers"));

	return actions;
}

const ModBlockActions BlockTestEvent::RuntimeInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should only run once for each stack; used for local containers"));

	return actions;
}

const ModBlockActions BlockTestEvent::RuntimePostInit() const
{
	ModBlockActions actions;
	actions.AddAction(ModBlockActionLogInfo("this should run for every block that exists during post init; used for local containers"));

	return actions;
}

const std::vector<BlockArgumentInitializer> BlockTestEvent::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pull events window"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
