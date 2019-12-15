#include "ThreadHandler.h"
#include "stacking/Plane.h"
#include "RuntimeHandler.h"
#include "registries/BlockRegistry.h"
#include "handlers/Logger.h"
#include "handlers/runtime/VariableHandler.h"
#include <iostream>

void ThreadRuntimeThread(Plane* plane, unsigned long long stack, bool* running, bool* done)
{
	std::vector<unsigned int> markLocations;
	std::vector<std::string> markLocationNames;
	std::vector<unsigned int> selectionForBlocks;
	std::vector<unsigned int> selectionForStacks;
	std::vector<bool> selectionForBlockSync;

	selectionForBlocks.push_back(0);
	selectionForStacks.push_back(stack);
	selectionForBlockSync.push_back(false);

	std::chrono::time_point<std::chrono::steady_clock> last = std::chrono::high_resolution_clock::now();

	while (RuntimeHandler::Running && *running)
	{
		const RegBlock* regBlock = BlockRegistry::GetBlock(plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUnlocalizedName());
		BlockRuntimeReturn args = plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();

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
				*done = true;
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
				*done = true;
				return;
			}

			RuntimeHandler::ManualRenderingEnabled = varText[0] == '1';
		}
		else if (regBlock->UnlocalizedName == "vin_execution_render_frame")
		{
			if (RuntimeHandler::ManualRenderingEnabled)
			{
				RuntimeHandler::ManualRenderFrame = true;

				while (RuntimeHandler::ManualRenderFrame)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
			else
			{
				Logger::Warn("can not render unless manual rendering is enabled");
			}
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
					*done = true;
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
		else if (regBlock->UnlocalizedName == "vin_execution_goto_if")
		{
			std::string indexText = (*args.Args)[0].substr(1, (*args.Args)[0].length() - 1);
			std::string conditionText = (*args.Args)[1].substr(1, (*args.Args)[1].length() - 1);

			bool condition = false;
			if ((*args.Args)[1][0] == '0')
			{
				condition = conditionText == "1";
			}
			else
			{
				bool* value = VariableHandler::GetBool(conditionText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + conditionText + "\" does not exist");
					*done = true;
					return;
				}

				condition = *value;
			}

			if (condition)
			{
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
						*done = true;
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
		}
		else if (regBlock->UnlocalizedName == "vin_thread_function_call")
		{
			std::string functionText = (*args.Args)[0].substr(1, (*args.Args)[0].length() - 1);

			std::string functionName = std::string();

			if ((*args.Args)[0][0] == '0')
				functionName = functionText;
			else
			{
				std::string* value = VariableHandler::GetString(functionText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + functionText + "\" does not exist");
					*done = true;
					return;
				}

				functionName = *value;
			}

			int searchResult = RuntimeHandler::PerformFunctionSearch(functionName);
			if (searchResult == -1)
			{
				Logger::Error("function \"" + functionName + "\" does not exist");
				*done = true;
				return;
			}

			selectionForBlocks[0]++;

			selectionForBlocks.insert(selectionForBlocks.begin(), 0);
			selectionForStacks.insert(selectionForStacks.begin(), searchResult);
		}
		else if (regBlock->Execute == nullptr)
		{
			Logger::Warn("no execution for block \"" + regBlock->UnlocalizedName + "\"");
		}
		else
		{
			BlockRuntimeReturn args = plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();

			if (!(*regBlock->Execute)(args.Args))
			{
				Logger::Error("block execution failed for block \"" + std::to_string(selectionForBlocks[0]) + "\"");
				*done = true;
				return;
			}
		}

		if (selectionForBlockSync[0])
		{
			while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last).count() < 5)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			
			last = std::chrono::high_resolution_clock::now();
		}

		selectionForBlocks[0]++;

		if (selectionForBlocks[0] >= plane->GetStack(selectionForStacks[0])->GetBlockCount())
		{
			if (selectionForStacks.size() > 1)
			{
				selectionForStacks.erase(selectionForStacks.begin());
				selectionForBlocks.erase(selectionForBlocks.begin());

				if (selectionForBlocks[0] >= plane->GetStack(selectionForStacks[0])->GetBlockCount())
				{
					*running = false;
				}
			}
			else
			{
				*running = false;
			}
		}
	}

	*done = true;
}

void ThreadHandler::Alloc(Plane* plane)
{
	m_activeThreads = new std::vector<std::thread*>();
	m_activeThreadIds = new std::vector<unsigned long long>();
	m_activeThreadRunning = new std::vector<bool*>();
	m_activeThreadDone = new std::vector<bool*>();
	m_counter = 0;

	m_plane = plane;
}

void ThreadHandler::Dealloc()
{
	for (unsigned int i = 0; i < m_activeThreads->size(); i++)
		delete (*m_activeThreads)[i];

	for (unsigned int i = 0; i < m_activeThreadRunning->size(); i++)
		delete (*m_activeThreadRunning)[i];

	for (unsigned int i = 0; i < m_activeThreadDone->size(); i++)
		delete (*m_activeThreadDone)[i];

	delete m_activeThreads;
	delete m_activeThreadIds;
	delete m_activeThreadRunning;
	delete m_activeThreadDone;
}

void ThreadHandler::KillJoinAll()
{
	for (unsigned int i = 0; i < m_activeThreads->size(); i++)
		KillThread((*m_activeThreadIds)[i]);

	for (unsigned int i = 0; i < m_activeThreadDone->size(); i++)
	{
		while (!*(*m_activeThreadDone)[i])
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

unsigned long long ThreadHandler::SummonThread(unsigned long long stackIndex)
{
	m_activeThreadIds->push_back(++m_counter);

	bool* running = new bool(true);
	m_activeThreadRunning->push_back(running);

	bool* done = new bool(false);
	m_activeThreadDone->push_back(done);

	std::thread* run = new std::thread(ThreadRuntimeThread, m_plane, stackIndex, running, done);
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
			*((*m_activeThreadRunning)[i]) = false;

			return true;
		}
	}

	return false;
}

std::vector<std::thread*>* ThreadHandler::m_activeThreads;

std::vector<unsigned long long>* ThreadHandler::m_activeThreadIds;

std::vector<bool*>* ThreadHandler::m_activeThreadRunning;

std::vector<bool*>* ThreadHandler::m_activeThreadDone;

unsigned long long ThreadHandler::m_counter;

Plane* ThreadHandler::m_plane;
