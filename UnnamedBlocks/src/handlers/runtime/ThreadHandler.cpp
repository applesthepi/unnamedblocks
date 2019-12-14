#include "ThreadHandler.h"
#include "stacking/Plane.h"
#include "RuntimeHandler.h"
#include "registries/BlockRegistry.h"
#include "handlers/Logger.h"
#include "handlers/runtime/VariableHandler.h"
#include <iostream>

void ThreadRuntimeThread(Plane* plane, unsigned long long stack, bool* running, std::mutex* runningMutex, bool* done, std::mutex* doneMutex)
{
	std::vector<unsigned int> markLocations;
	std::vector<std::string> markLocationNames;
	std::vector<unsigned int> selectionForBlocks;
	std::vector<bool> selectionForBlockSync;

	selectionForBlocks.push_back(0);
	selectionForBlockSync.push_back(false);

	std::chrono::time_point<std::chrono::steady_clock> last = std::chrono::high_resolution_clock::now();

	runningMutex->lock();
	while (RuntimeHandler::Running && *running)
	{
		runningMutex->unlock();

		const RegBlock* regBlock = BlockRegistry::GetBlock(plane->GetStack(stack)->GetBlock(selectionForBlocks[0])->GetUnlocalizedName());
		BlockRuntimeReturn args = plane->GetStack(stack)->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();

		if (regBlock->UnlocalizedName == "vin_execution_mark")
		{
			std::string indexText = (*args.Args)[0].substr(1, (*args.Args)[0].length() - 1);

			markLocationNames.push_back(indexText);
			markLocations.push_back(selectionForBlocks[0]);
		}
		else if (regBlock->UnlocalizedName == "vin_execution_block_sync")
		{
			std::string varText = (*args.Args)[0].substr(1, (*args.Args)[0].length() - 1);

			if ((*args.Args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return;
			}

			selectionForBlockSync[0] = varText[0] == '1';
		}
		else if (regBlock->UnlocalizedName == "vin_execution_manual_render")
		{
			std::string varText = (*args.Args)[0].substr(1, (*args.Args)[0].length() - 1);

			if ((*args.Args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return;
			}

			RuntimeHandler::ManualRenderingEnabled = varText[0] == '1';
		}
		else if (regBlock->UnlocalizedName == "vin_execution_render_frame")
		{
			if (RuntimeHandler::ManualRenderingEnabled)
				RuntimeHandler::ManualRender();
		}
		else if (regBlock->UnlocalizedName == "vin_execution_goto")
		{
			std::string indexText = (*args.Args)[0].substr(1, (*args.Args)[0].length() - 1);

			std::string name = std::string();
			if ((*args.Args)[0][0] == '0')
			{
				name = indexText;
			}
			else
			{
				std::string* value = VariableHandler::GetString(indexText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + indexText + "\" does not exist");
					return;
				}

				name = *value;
			}

			for (unsigned int i = 0; i < markLocationNames.size(); i++)
			{
				if (markLocationNames[i] == name)
				{
					selectionForBlocks[0] = markLocations[i];
					break;
				}
			}
		}
		else if (regBlock->Execute == nullptr)
		{
			Logger::Warn("no execution for block \"" + regBlock->UnlocalizedName + "\"");
		}
		else
		{
			BlockRuntimeReturn args = plane->GetStack(stack)->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();

			if (!(*regBlock->Execute)(args.Args))
			{
				Logger::Error("block execution failed for block \"" + std::to_string(selectionForBlocks[0]) + "\"");
				return;
			}
		}

		if (selectionForBlockSync[0])
		{
			while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last).count() < 5)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			std::cout << "times pasted: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last).count() << std::endl;
			last = std::chrono::high_resolution_clock::now();
		}

		selectionForBlocks[0]++;

		runningMutex->lock();

		if (selectionForBlocks[0] >= plane->GetStack(stack)->GetBlockCount())
		{
			*running = false;
		}
	}

	runningMutex->unlock();

	doneMutex->lock();
	*done = true;
	doneMutex->unlock();
}

void ThreadHandler::Alloc(Plane* plane)
{
	m_activeThreads = new std::vector<std::thread*>();
	m_activeThreadIds = new std::vector<unsigned long long>();
	m_activeThreadRunning = new std::vector<bool*>();
	m_activeThreadRunningMutex = new std::vector<std::mutex*>();
	m_activeThreadDone = new std::vector<bool*>();
	m_activeThreadDoneMutex = new std::vector<std::mutex*>();

	m_plane = plane;
}

void ThreadHandler::Dealloc()
{
	for (unsigned int i = 0; i < m_activeThreads->size(); i++)
	{
		delete (*m_activeThreads)[i];
	}

	for (unsigned int i = 0; i < m_activeThreadRunning->size(); i++)
		delete (*m_activeThreadRunning)[i];

	for (unsigned int i = 0; i < m_activeThreadRunningMutex->size(); i++)
		delete (*m_activeThreadRunningMutex)[i];

	for (unsigned int i = 0; i < m_activeThreadDone->size(); i++)
		delete (*m_activeThreadDone)[i];

	for (unsigned int i = 0; i < m_activeThreadDoneMutex->size(); i++)
		delete (*m_activeThreadDoneMutex)[i];

	delete m_activeThreads;
	delete m_activeThreadIds;
	delete m_activeThreadRunning;
	delete m_activeThreadRunningMutex;
	delete m_activeThreadDone;
	delete m_activeThreadDoneMutex;
}

void ThreadHandler::KillJoinAll()
{
	for (unsigned int i = 0; i < m_activeThreads->size(); i++)
	{
		KillThread((*m_activeThreadIds)[i]);
	}

	for (unsigned int i = 0; i < m_activeThreadDone->size(); i++)
	{
		(*m_activeThreadDoneMutex)[i]->lock();

		while (!*(*m_activeThreadDone)[i])
		{
			(*m_activeThreadDoneMutex)[i]->unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			(*m_activeThreadDoneMutex)[i]->lock();
		}

		(*m_activeThreadDoneMutex)[i]->unlock();
	}
}

unsigned long long ThreadHandler::SummonThread(unsigned long long stackIndex)
{
	m_activeThreadIds->push_back(++m_counter);

	bool* running = new bool(true);
	m_activeThreadRunning->push_back(running);

	std::mutex* runningMutex = new std::mutex();
	m_activeThreadRunningMutex->push_back(runningMutex);

	bool* done = new bool(false);
	m_activeThreadDone->push_back(done);

	std::mutex* doneMutex = new std::mutex();
	m_activeThreadDoneMutex->push_back(doneMutex);

	std::thread* run = new std::thread(ThreadRuntimeThread, m_plane, stackIndex, running, runningMutex, done, doneMutex);
	run->detach();
	m_activeThreads->push_back(run);

	return m_counter;
}

bool ThreadHandler::KillThread(unsigned long long threadId)
{
	for (unsigned long long i = 0; i < m_activeThreadIds->size(); i++)
	{
		if ((*m_activeThreadIds)[i] == threadId)
		{
			(*m_activeThreadRunningMutex)[i]->lock();
			*((*m_activeThreadRunning)[i]) = false;
			(*m_activeThreadRunningMutex)[i]->unlock();

			return true;
		}
	}

	return false;
}

std::vector<std::thread*>* ThreadHandler::m_activeThreads;

std::vector<unsigned long long>* ThreadHandler::m_activeThreadIds;

std::vector<bool*>* ThreadHandler::m_activeThreadRunning;

std::vector<std::mutex*>* ThreadHandler::m_activeThreadRunningMutex;

std::vector<bool*>* ThreadHandler::m_activeThreadDone;

std::vector<std::mutex*>* ThreadHandler::m_activeThreadDoneMutex;

unsigned long long ThreadHandler::m_counter;

Plane* ThreadHandler::m_plane;
