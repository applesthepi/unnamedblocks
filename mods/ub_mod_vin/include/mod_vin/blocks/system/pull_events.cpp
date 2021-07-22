#include "BlockSystemPullEvents.hpp"
#include "BlockSystemCreateWindow.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

static void ExecuteRelease(ModBlockPass* pass)
{
	//WindowHandler* handler = (WindowHandler*)pass->CustomGet(pass->GetReal(0));
	//sf::RenderWindow* window = handler->GetWindow();
	//
	//sf::Event ev;

	//while (window->pollEvent(ev))
	//{
	//	if (ev.type == sf::Event::Closed)
	//	{
	//		window->close();
	//		pass->Stop();
	//		return;
	//	}
	//	else if (ev.type == sf::Event::MouseWheelScrolled)
	//	{
	//		handler->AddScroll(ev.mouseWheelScroll.delta);
	//	}
	//}
}

static void ExecuteDebug(ModBlockPass* pass)
{
	ExecuteRelease(pass);
}

const char* BlockSystemPullEvents::GetUnlocalizedName() const
{
	return "vin_system_pull_events";
}

const char* BlockSystemPullEvents::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemPullEvents::PullExecuteDebug() const
{
	return ExecuteDebug;
}

blockExecution BlockSystemPullEvents::PullExecuteRelease() const
{
	return ExecuteRelease;
}

const std::vector<BlockArgumentInitializer> BlockSystemPullEvents::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "window pull events"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "window"));

	return args;
}
