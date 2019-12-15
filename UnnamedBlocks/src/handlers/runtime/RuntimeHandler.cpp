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
		}

		std::cout << "manual rendering: " << RuntimeHandler::ManualRenderingEnabled << std::endl;

		if (!RuntimeHandler::ManualRenderingEnabled)
			RuntimeHandler::ManualRender();
	}

	//shutdown
	delete RuntimeHandler::Window;
	RuntimeHandler::CleanUp();
}

void RuntimeHandler::Run(Plane* planeCopy)
{
	if (Running)
		return;

	Running = true;
	ManualRenderingEnabled = false;

	m_planeCopy = planeCopy;

	std::vector<unsigned long long> locations;
	for (unsigned long long i = 0; i < planeCopy->GetStackCount(); i++)
	{
		if (planeCopy->GetStack(i)->GetBlock(0)->GetUnlocalizedName() == "vin_thread_open")
		{
			locations.push_back(i);
		}
	}

	if (locations.size() == 0)
	{
		Logger::Error("you must have atleast 1 opening for the program to run");
		Running = false;
		return;
	}

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

void RuntimeHandler::ManualRender()
{
	m_renderMutex.lock();
	Window->clear();

	ObjectHandler::FrameUpdate(Window);
	ObjectHandler::Render(Window);

	Window->display();
	m_renderMutex.unlock();
}

bool RuntimeHandler::Running;

bool RuntimeHandler::ManualRenderingEnabled;

sf::RenderWindow* RuntimeHandler::Window;

Plane* RuntimeHandler::m_planeCopy;

std::thread* RuntimeHandler::m_runningThread;

std::mutex RuntimeHandler::m_renderMutex;
