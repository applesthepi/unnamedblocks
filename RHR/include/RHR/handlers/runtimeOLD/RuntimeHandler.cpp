#include "RuntimeHandler.h"
#include "ObjectHandler.h"
#include "ThreadHandler.h"
#include "VariableHandler.h"
#include "ByteHandler.h"
#include "handlers/Logger.h"
#include "StatmentIf.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdint.h>
#include <ctime>

void ThreadWindowManager(RuntimeHandler* runtime, std::atomic<bool>* initialized)
{
	runtime->Window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Unnamed Blocks Runtime");
	runtime->Window->setVerticalSyncEnabled(false);
	runtime->Window->setFramerateLimit(240);

	*initialized = true;

	while (runtime->Running.load())
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

	runtime->Window->close();

	//shutdown
	runtime->CleanUp();

	delete runtime->Window;
}

RuntimeHandler::RuntimeHandler(ThreadHandler* thread, ObjectHandler* object, VariableHandler* variable, ByteHandler* byte)
	:m_threadHandler(thread), m_objectHandler(object), m_variableHandler(variable), m_byteHandler(byte)
{
}

void RuntimeHandler::Reset()
{
	m_stackFunctions.clear();
	m_stackIndices.clear();
	m_stackFunctionIfStatments.clear();

	Running.store(false);
	ManualRenderingEnabled.store(false);
	
	m_appMutexCount = 0;

	for (uint64_t i = 0; i < m_appMutex.size(); i++)
		delete m_appMutex[i];

	m_appMutex.clear();
}

void RuntimeHandler::Run(Plane* planeCopy, BlockRegistry* registry)
{
	if (Running.load())
		return;

	std::vector<unsigned long long> locations;
	for (unsigned long long i = 0; i < planeCopy->GetStackCount(); i++)
	{
		Stack* currentStack = planeCopy->GetStack(i);
		currentStack->IndexVariables();
		m_stackFunctionIfStatments.push_back(ProcessIfStatments(currentStack));

		if (currentStack->GetBlock(0)->GetUnlocalizedName() == "vin_thread_open")
		{
			locations.push_back(i);
		}
		else if (currentStack->GetBlock(0)->GetUnlocalizedName() == "vin_thread_function_define")
		{
			//const RegBlock* regBlock = registry->GetBlock(planeCopy->GetStack(i)->GetBlock(0)->GetUnlocalizedName());
			BlockRuntimeReturn args = planeCopy->GetStack(i)->GetBlock(0)->GetUsedArgumentsRuntime();

			std::string functionText = (*args.Args)[0].Value;

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

	{
		m_appMutexCount = 0;

		for (uint64_t i = 0; i < m_appMutex.size(); i++)
			delete m_appMutex[i];

		m_appMutex.clear();
	}

	std::atomic<bool> initialized(false);

	m_runningThread = new std::thread(ThreadWindowManager, this, &initialized);
	m_runningThread->detach();

	while (!initialized.load())
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	for (unsigned int i = 0; i < locations.size(); i++)
		m_threadHandler->SummonThread(locations[i], this, m_variableHandler, registry, false, 0);
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

const Plane* RuntimeHandler::GetPlane()
{
	return m_planeCopy;
}

uint64_t RuntimeHandler::MutexCreate()
{
	m_appMutexCount++;

	m_appMutex.push_back(new std::shared_mutex());
	m_appMutexIdx.push_back(m_appMutexCount);

	return m_appMutexCount;
}

bool RuntimeHandler::MutexFree(uint64_t idx)
{
	for (uint64_t i = 0; i < m_appMutex.size(); i++)
	{
		if (m_appMutexIdx[i] == idx)
		{
			if (m_appMutex[i]->try_lock())
			{
				m_appMutex[i]->unlock();
				delete m_appMutex[i];

				m_appMutexIdx.erase(m_appMutexIdx.begin() + i);
				m_appMutex.erase(m_appMutex.begin() + i);

				return true;
			}
			else
			{
				Logger::Error("tried to free mutex that was locked");
				return false;
			}
		}
	}

	Logger::Error("mutex \"" + std::to_string(idx) + "\" does not exist");
	return false;
}

bool RuntimeHandler::MutexLockShared(uint64_t idx)
{
	for (uint64_t i = 0; i < m_appMutex.size(); i++)
	{
		if (m_appMutexIdx[i] == idx)
		{
			try
			{
				m_appMutex[i]->lock_shared();
			}
			catch (std::system_error&)
			{
				Logger::Error("mutex failed to lock");
				return false;
			}

			return true;
		}
	}

	Logger::Error("mutex \"" + std::to_string(idx) + "\" does not exist");
	return false;
}

bool RuntimeHandler::MutexReleaseShared(uint64_t idx)
{
	for (uint64_t i = 0; i < m_appMutex.size(); i++)
	{
		if (m_appMutexIdx[i] == idx)
		{
			m_appMutex[i]->unlock_shared();
			return true;
		}
	}

	Logger::Error("mutex \"" + std::to_string(idx) + "\" does not exist");
	return false;
}

bool RuntimeHandler::MutexLockUnique(uint64_t idx)
{
	for (uint64_t i = 0; i < m_appMutex.size(); i++)
	{
		if (m_appMutexIdx[i] == idx)
		{
			try
			{
				m_appMutex[i]->lock();
			}
			catch (std::system_error&)
			{
				Logger::Error("mutex failed to lock");
				return false;
			}

			return true;
		}
	}

	Logger::Error("mutex \"" + std::to_string(idx) + "\" does not exist");
	return false;
}

bool RuntimeHandler::MutexReleaseUnique(uint64_t idx)
{
	for (uint64_t i = 0; i < m_appMutex.size(); i++)
	{
		if (m_appMutexIdx[i] == idx)
		{
			m_appMutex[i]->unlock();
			return true;
		}
	}

	Logger::Error("mutex \"" + std::to_string(idx) + "\" does not exist");
	return false;
}

const std::vector<StatmentIf>* RuntimeHandler::GetIfStatments(uint32_t stackIdx)
{
	return &(m_stackFunctionIfStatments[stackIdx]);
}

std::vector<StatmentIf> RuntimeHandler::ProcessIfStatments(Stack* stack, bool muteErrors)
{
	std::vector<StatmentIf> ifStatments;
	std::vector<StatmentIf> currentIfs;
	std::vector<std::vector<uint32_t>> currentIfElse;

	for (uint32_t a = 0; a < stack->GetBlockCount(); a++)
	{
		if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_if")
		{
			currentIfs.push_back(StatmentIf());
			currentIfs.back().Location = a;

			continue;
		}
		
		if (currentIfs.size() > 0)
		{
			if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_if_end")
			{
				currentIfs.back().LocationEnd = a;

				if (currentIfs.back().HasElseIf)
				{
					currentIfs.back().LocationElseIf = currentIfElse.back();
					currentIfElse.pop_back();
				}

				ifStatments.push_back(currentIfs.back());
				currentIfs.pop_back();
			}
			else if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_else")
			{
				currentIfs.back().HasElse = true;
				currentIfs.back().LocationElse = a;
			}
			else if (stack->GetBlock(a)->GetUnlocalizedName() == "vin_execution_else_if")
			{
				if (currentIfs.back().HasElse)
				{
					if (!muteErrors)
						Logger::Error("failed to construct if statement track, you can not have an \"else if\" block after an \"else\" block in an if statement");

					return std::vector<StatmentIf>();
				}

				if (!currentIfs.back().HasElseIf)
					currentIfElse.push_back(std::vector<uint32_t>());

				currentIfs.back().HasElseIf = true;
				currentIfElse.back().push_back(a);
			}
		}
	}

	if (currentIfs.size() > 0)
	{
		if (!muteErrors)
			Logger::Error("failed to construct if statement track, unbalanced statements");

		return std::vector<StatmentIf>();
	}

	return ifStatments;
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