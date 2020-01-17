#include "RuntimeHandler.h"


#include "ObjectHandler.h"
#include "ThreadHandler.h"
#include "VariableHandler.h"
#include "ByteHandler.h"
#include "handlers/Logger.h"

#include <SFML/Graphics.hpp>
#include <iostream>

void ThreadWindowManager()
{
	RuntimeHandler::Window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Unnamed Blocks Runtime");
	RuntimeHandler::Window->setVerticalSyncEnabled(true);

	while (RuntimeHandler::Running)
	{
		sf::Event ev;
		while (RuntimeHandler::Window->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				RuntimeHandler::Window->close();
				RuntimeHandler::Running = false;
			}
			else if (ev.type == sf::Event::MouseWheelMoved)
			{
				RuntimeHandler::AddScroll(ev.mouseWheel.delta);
			}
		}

		if (!RuntimeHandler::ManualRenderingEnabled)
			RuntimeHandler::ManualRender();
		else if (RuntimeHandler::ManualRenderFrame)
		{
			RuntimeHandler::ManualRender();
			RuntimeHandler::ManualRenderFrame = false;
		}
	}

	//shutdown
	delete RuntimeHandler::Window;
	RuntimeHandler::CleanUp();
}

void RuntimeHandler::Run(Plane* planeCopy)
{
	if (Running)
		return;

	std::vector<unsigned long long> locations;
	for (unsigned long long i = 0; i < planeCopy->GetStackCount(); i++)
	{
		if (planeCopy->GetStack(i)->GetBlock(0)->GetUnlocalizedName() == "vin_thread_open")
		{
			locations.push_back(i);
		}
		else if (planeCopy->GetStack(i)->GetBlock(0)->GetUnlocalizedName() == "vin_thread_function_define")
		{
			const RegBlock* regBlock = BlockRegistry::MainRegistry->GetBlock(planeCopy->GetStack(i)->GetBlock(0)->GetUnlocalizedName());
			BlockRuntimeReturn args = planeCopy->GetStack(i)->GetBlock(0)->GetUsedArgumentsRuntime();

			std::string functionText = (*args.Args)[0].Value.substr(1, (*args.Args)[0].Value.length() - 1);

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::VAR)
			{
				Logger::Error("expecting text only! got \"" + functionText + "\"");
				return;
			}

			m_stackFunctions.push_back(functionText);
			m_stackIndices.push_back(i);
		}
	}

	if (locations.size() == 0)
	{
		Logger::Error("you must have atleast 1 opening for the program to run");
		return;
	}

	Running = true;
	ManualRenderingEnabled = false;

	m_planeCopy = planeCopy;

	ThreadHandler::Alloc(planeCopy);
	ObjectHandler::Alloc();
	VariableHandler::Alloc();
	ByteHandler::Alloc();

	for (unsigned int i = 0; i < locations.size(); i++)
	{
		ThreadHandler::SummonThread(locations[i]);
	}

	m_runningThread = new std::thread(ThreadWindowManager);
	m_runningThread->detach();
}

void RuntimeHandler::ManualRender()
{
	m_renderMutex.lock();
	Window->clear();

	ObjectHandler::FrameUpdate(Window);
	ObjectHandler::Render(Window);

	Window->display();
	m_renderMutex.unlock();
}

int RuntimeHandler::GetScrolled()
{
	m_scrollMutex.lock();
	int scrolled = m_scrolled;
	m_scrollMutex.unlock();

	return scrolled;
}

void RuntimeHandler::AddScroll(int value)
{
	m_scrollMutex.lock();
	m_scrolled += value;
	m_scrollMutex.unlock();
}

void RuntimeHandler::ResetScrolled()
{
	m_scrollMutex.lock();
	m_scrolled = 0;
	m_scrollMutex.unlock();
}

int RuntimeHandler::PerformFunctionSearch(std::string functionName)
{
	for (unsigned int i = 0; i < m_stackFunctions.size(); i++)
	{
		if (m_stackFunctions[i] == functionName)
			return m_stackIndices[i];
	}

	return -1;
}

void RuntimeHandler::CleanUp()
{
	ThreadHandler::KillJoinAll();

	ThreadHandler::Dealloc();
	ObjectHandler::Dealloc();
	VariableHandler::Dealloc();
	ByteHandler::Dealloc();

	delete m_runningThread;
	delete m_planeCopy;
}

bool RuntimeHandler::Running;

bool RuntimeHandler::ManualRenderFrame;

bool RuntimeHandler::ManualRenderingEnabled;

sf::RenderWindow* RuntimeHandler::Window;

Plane* RuntimeHandler::m_planeCopy;

std::thread* RuntimeHandler::m_runningThread;

std::mutex RuntimeHandler::m_renderMutex;

std::mutex RuntimeHandler::m_scrollMutex;

int RuntimeHandler::m_scrolled;

std::vector<unsigned int> RuntimeHandler::m_stackIndices;

std::vector<std::string> RuntimeHandler::m_stackFunctions;
