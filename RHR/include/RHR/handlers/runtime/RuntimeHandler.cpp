#include "RuntimeHandler.h"
#include "ObjectHandler.h"
#include "ThreadHandler.h"
#include "VariableHandler.h"
#include "ByteHandler.h"
#include "handlers/Logger.h"

#include <SFML/Graphics.hpp>
#include <iostream>

void ThreadWindowManager(RuntimeHandler* runtime)
{
	runtime->Window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Unnamed Blocks Runtime");
	runtime->Window->setVerticalSyncEnabled(false);
	runtime->Window->setFramerateLimit(240);

	while (runtime->Running)
	{
		sf::Event ev;
		while (runtime->Window->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				runtime->Window->close();
				runtime->Running = false;
			}
			else if (ev.type == sf::Event::MouseWheelMoved)
			{
				runtime->AddScroll(ev.mouseWheel.delta);
			}
		}

		if (!runtime->ManualRenderingEnabled)
			runtime->ManualRender();
		else if (runtime->ManualRenderFrame)
		{
			runtime->ManualRender();
			runtime->ManualRenderFrame = false;
		}
	}

	//shutdown
	delete runtime->Window;
	runtime->CleanUp();
}

RuntimeHandler::RuntimeHandler(ThreadHandler* thread, ObjectHandler* object, VariableHandler* variable, ByteHandler* byte)
	:m_threadHandler(thread), m_objectHandler(object), m_variableHandler(variable), m_byteHandler(byte)
{
}

void RuntimeHandler::Reset()
{
	m_stackFunctions.clear();
	m_stackIndices.clear();

	Running = false;
	ManualRenderingEnabled = false;
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

	for (unsigned int i = 0; i < locations.size(); i++)
	{
		m_threadHandler->SummonThread(locations[i], this, m_variableHandler);
	}

	m_runningThread = new std::thread(ThreadWindowManager, this);
	m_runningThread->detach();
}

void RuntimeHandler::ManualRender()
{
	m_renderMutex.lock();
	Window->clear();

	m_objectHandler->FrameUpdate(Window);
	m_objectHandler->Render(Window);

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

RuntimeHandler& RuntimeHandler::operator=(const RuntimeHandler& other)
{
	return *this;
}

void RuntimeHandler::CleanUp()
{
	m_threadHandler->KillJoinAll();

	delete m_runningThread;
	delete m_planeCopy;
}
