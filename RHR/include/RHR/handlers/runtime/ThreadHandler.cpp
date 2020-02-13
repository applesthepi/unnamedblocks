#include "ThreadHandler.h"
#include "stacking/Plane.h"
#include "RuntimeHandler.h"
#include "registries/BlockRegistry.h"
#include "handlers/Logger.h"
#include "handlers/runtime/VariableHandler.h"
#include "StatmentIf.h"
#include <iostream>

#define INVALID_IF_CALLSTACK "invalid if callstack detected!"

void ThreadRuntimeThread(Plane* plane, unsigned long long stack, std::atomic<bool>* running, std::atomic<bool>* done, RuntimeHandler* runtime, VariableHandler* variables, BlockRegistry* registry, uint64_t threadIdx)
{
	srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

	std::vector<unsigned int> markLocations;
	std::vector<std::string> markLocationNames;
	std::vector<unsigned int> selectionForBlocks;
	std::vector<unsigned int> selectionForStacks;
	std::vector<bool> selectionForBlockSync;

	const std::vector<StatmentIf>* ifStatments = runtime->GetIfStatments(stack);

	selectionForBlocks.push_back(0);
	selectionForStacks.push_back(stack);
	selectionForBlockSync.push_back(false);

	std::vector<const StatmentIf*> ifStack;
	std::vector<uint32_t> ifElseStack;
	std::vector<bool> ifStackAccepted;

#ifdef POSIX
	std::chrono::time_point<std::chrono::system_clock> last = std::chrono::high_resolution_clock::now();
#else
	std::chrono::time_point<std::chrono::steady_clock> last = std::chrono::high_resolution_clock::now();
#endif

#ifdef POSIX
	std::chrono::time_point<std::chrono::system_clock> inTimer = std::chrono::high_resolution_clock::now();
#else
	std::chrono::time_point<std::chrono::steady_clock> inTimer = std::chrono::high_resolution_clock::now();
#endif

	while (runtime->Running && *running)
	{
		const RegBlock* regBlock = registry->GetBlock(plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUnlocalizedName());
		BlockRuntimeReturn args = plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();
		std::string specialIdx = std::to_string(selectionForStacks.front()) + "_" + std::to_string(threadIdx) + "_";

		if (regBlock->UnlocalizedName == "vin_execution_mark")
		{
			std::string indexText = (*args.Args)[0].Value;

			markLocationNames.push_back(indexText);
			markLocations.push_back(selectionForBlocks[0]);
		}
		else if (regBlock->UnlocalizedName == "vin_execution_block_sync")
		{
			std::string varText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::VAR)
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				done->store(true);
				return;
			}

			selectionForBlockSync[0] = varText[0] == '1';
		}
		else if (regBlock->UnlocalizedName == "vin_execution_block_manual_render")
		{
			std::string varText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::VAR)
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				done->store(true);
				return;
			}

			runtime->ManualRenderingEnabled = varText[0] == '1';
		}
		else if (regBlock->UnlocalizedName == "vin_execution_block_render_frame")
		{
			if (runtime->ManualRenderingEnabled)
			{
				runtime->ManualRenderFrame = true;

				while (runtime->ManualRenderFrame)
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
			std::string indexText = (*args.Args)[0].Value;

			std::string name = std::string();
			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				name = indexText;
			}
			else
			{
				std::string* value = variables->GetString((specialIdx + indexText).c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + indexText + "\" does not exist");
					done->store(true);
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
			std::string indexText = (*args.Args)[0].Value;
			std::string conditionText = (*args.Args)[1].Value;

			bool condition = false;
			if ((*args.Args)[1].Mode == BlockArgumentVariableMode::RAW)
			{
				condition = conditionText == "1";
			}
			else
			{
				bool* value = variables->GetBool((specialIdx + conditionText).c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + conditionText + "\" does not exist");
					done->store(true);
					return;
				}

				condition = *value;
			}

			if (condition)
			{
				std::string name = std::string();
				if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
				{
					name = indexText;
				}
				else
				{
					std::string* value = variables->GetString((specialIdx + indexText).c_str());
					if (value == nullptr)
					{
						Logger::Error("variable \"" + indexText + "\" does not exist");
						done->store(true);
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
			std::string functionText = (*args.Args)[0].Value;

			std::string functionName;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
				functionName = functionText;
			else
			{
				std::string* value = variables->GetString((specialIdx + functionText).c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + functionText + "\" does not exist");
					done->store(true);
					return;
				}

				functionName = *value;
			}

			int searchResult = runtime->PerformFunctionSearch(functionName);
			if (searchResult == -1)
			{
				Logger::Error("function \"" + functionName + "\" does not exist");
				done->store(true);
				return;
			}

			double gotValue = 0.0;
			if ((*args.Args)[1].Mode == BlockArgumentVariableMode::VAR)
			{
				double* attempt = variables->GetReal((specialIdx + (*args.Args)[1].Value).c_str());

				if (attempt == nullptr)
				{
					Logger::Error("variable \"" + (*args.Args)[1].Value + "\" does not exist");
					done->store(true);
					return;
				}

				gotValue = *attempt;
			}
			else
				gotValue = std::stod((*args.Args)[1].Value);

			selectionForBlocks[0]++;

			selectionForBlocks.insert(selectionForBlocks.begin(), 0);
			selectionForStacks.insert(selectionForStacks.begin(), searchResult);

			BlockRuntimeReturn funArgs = plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();

			variables->StackReal((specialIdx + (*funArgs.Args)[1].Value).c_str());
			variables->SetReal((specialIdx + (*funArgs.Args)[1].Value).c_str(), gotValue);
		}
		else if (regBlock->UnlocalizedName == "vin_execution_if")
		{
			std::string conditionText = (*args.Args)[0].Value;

			bool condition = false;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				condition = conditionText == "1";
			}
			else
			{
				bool* value = variables->GetBool((specialIdx + conditionText).c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + conditionText + "\" does not exist");
					done->store(true);
					return;
				}

				condition = *value;
			}

			bool found = false;

			for (uint32_t i = 0; i < ifStatments->size(); i++)
			{
				if (ifStatments->at(i).Location == selectionForBlocks.front())
				{
					ifStack.push_back(&((*ifStatments)[i]));
					found = true;
					break;
				}
			}

			if (found)
			{
				ifElseStack.push_back(0);
				ifStackAccepted.push_back(false);

				if (!condition)
				{
					if (ifStack.back()->HasElseIf)
						selectionForBlocks.front() = ifStack.back()->LocationElseIf[ifElseStack.back()] - 1;
					else if (ifStack.back()->HasElse)
						selectionForBlocks.front() = ifStack.back()->LocationElse - 1;
					else
						selectionForBlocks.front() = ifStack.back()->LocationEnd - 1;
				}
				else
					ifStackAccepted.back() = true;
			}
			else
			{
				Logger::Error(INVALID_IF_CALLSTACK);
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_else_if")
		{
			std::string conditionText = (*args.Args)[0].Value;

			bool condition = false;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				condition = conditionText == "1";
			}
			else
			{
				bool* value = variables->GetBool((specialIdx + conditionText).c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + conditionText + "\" does not exist");
					done->store(true);
					return;
				}

				condition = *value;
			}

			if (ifStack.size() > 0)
			{
				if (ifStackAccepted.back())
					selectionForBlocks.front() = ifStack.back()->LocationEnd - 1;
				else
				{
					if (!condition)
					{
						if (ifStack.back()->HasElseIf)
						{
							ifElseStack.back()++;

							if (ifElseStack.back() >= ifStack.back()->LocationElseIf.size())
							{
								if (ifStack.back()->HasElse)
									selectionForBlocks.front() = ifStack.back()->LocationElse - 1;
								else
									selectionForBlocks.front() = ifStack.back()->LocationEnd - 1;
							}
							else
								selectionForBlocks.front() = ifStack.back()->LocationElseIf[ifElseStack.back()] - 1;
						}
						else if (ifStack.back()->HasElse)
							selectionForBlocks.front() = ifStack.back()->LocationElse - 1;
					}
					else
						ifStackAccepted.back() = true;
				}
			}
			else
			{
				Logger::Error(INVALID_IF_CALLSTACK);
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_else")
		{
			if (ifStack.size() > 0)
			{
				if (ifStackAccepted.back())
					selectionForBlocks.front() = ifStack.back()->LocationEnd - 1;
			}
			else
			{
				Logger::Error(INVALID_IF_CALLSTACK);
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_if_end")
		{
			if (ifStack.size() > 0)
			{
				ifStack.pop_back();
				ifElseStack.pop_back();
				ifStackAccepted.pop_back();
			}
			else
			{
				Logger::Error(INVALID_IF_CALLSTACK);
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_input_timer_get")
		{
			variables->SetReal((specialIdx + (*args.Args)[0].Value).c_str(), (double)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - inTimer).count());
		}
		else if (regBlock->UnlocalizedName == "vin_input_timer_reset")
		{
			inTimer = std::chrono::high_resolution_clock::now();
		}
		else if (regBlock->UnlocalizedName == "vin_execution_mutex_create")
		{
			std::string indexText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				Logger::Error("expecting var only! got \"" + indexText + "\"");
				done->store(true);
				return;
			}

			uint64_t mutexIdx = runtime->MutexCreate();

			if (!variables->SetReal((specialIdx + args.Args->at(0).Value).c_str(), mutexIdx))
			{
				Logger::Error("failed to create mutex");
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_mutex_free")
		{
			std::string indexText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				Logger::Error("expecting var only! got \"" + indexText + "\"");
				done->store(true);
				return;
			}

			double* gotIdx = variables->GetReal((specialIdx + args.Args->at(0).Value).c_str());

			if (gotIdx == nullptr)
			{
				Logger::Error("variable \"" + indexText + "\" does not exist");
				done->store(true);
				return;
			}

			if (!runtime->MutexFree(*gotIdx))
			{
				Logger::Error("failed to freeing mutex");
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_mutex_shared_lock")
		{
			std::string indexText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				Logger::Error("expecting var only! got \"" + indexText + "\"");
				done->store(true);
				return;
			}

			double* gotIdx = variables->GetReal((specialIdx + args.Args->at(0).Value).c_str());

			if (gotIdx == nullptr)
			{
				Logger::Error("variable \"" + indexText + "\" does not exist");
				done->store(true);
				return;
			}

			if (!runtime->MutexLockShared(*gotIdx))
			{
				Logger::Error("failed to lock mutex \"" + indexText + "\"");
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_mutex_shared_release")
		{
			std::string indexText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				Logger::Error("expecting var only! got \"" + indexText + "\"");
				done->store(true);
				return;
			}

			double* gotIdx = variables->GetReal((specialIdx + args.Args->at(0).Value).c_str());

			if (gotIdx == nullptr)
			{
				Logger::Error("variable \"" + indexText + "\" does not exist");
				done->store(true);
				return;
			}

			if (!runtime->MutexReleaseShared(*gotIdx))
			{
				Logger::Error("failed to release mutex \"" + indexText + "\"");
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_mutex_unique_lock")
		{
			std::string indexText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				Logger::Error("expecting var only! got \"" + indexText + "\"");
				done->store(true);
				return;
			}

			double* gotIdx = variables->GetReal((specialIdx + args.Args->at(0).Value).c_str());

			if (gotIdx == nullptr)
			{
				Logger::Error("variable \"" + indexText + "\" does not exist");
				done->store(true);
				return;
			}

			if (!runtime->MutexLockUnique(*gotIdx))
			{
				Logger::Error("failed to lock mutex \"" + indexText + "\"");
				done->store(true);
				return;
			}
		}
		else if (regBlock->UnlocalizedName == "vin_execution_mutex_unique_release")
		{
			std::string indexText = (*args.Args)[0].Value;

			if ((*args.Args)[0].Mode == BlockArgumentVariableMode::RAW)
			{
				Logger::Error("expecting var only! got \"" + indexText + "\"");
				done->store(true);
				return;
			}

			double* gotIdx = variables->GetReal((specialIdx + args.Args->at(0).Value).c_str());

			if (gotIdx == nullptr)
			{
				Logger::Error("variable \"" + indexText + "\" does not exist");
				done->store(true);
				return;
			}

			if (!runtime->MutexReleaseUnique(*gotIdx))
			{
				Logger::Error("failed to release mutex \"" + indexText + "\"");
				done->store(true);
				return;
			}
		}
		else if (regBlock->Execute == nullptr)
		{
			Logger::Warn("no execution for block \"" + regBlock->UnlocalizedName + "\"");
		}
		else
		{
			BlockRuntimeReturn args = plane->GetStack(selectionForStacks[0])->GetBlock(selectionForBlocks[0])->GetUsedArgumentsRuntime();

			if (!(*regBlock->Execute)(*(args.Args), specialIdx))
			{
				Logger::Error("block execution failed for block \"" + std::to_string(selectionForBlocks[0]) + "\"");
				done->store(true);
				return;
			}
		}

		if (selectionForBlockSync[0])
		{
			while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last).count() < 5)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
					running->store(false);
				}
			}
			else
			{
				running->store(false);
			}
		}
	}

	done->store(true);
}

ThreadHandler::ThreadHandler()
{
	m_activeThreads = new std::vector<std::thread*>();
	m_activeThreadIds = new std::vector<unsigned long long>();
	m_activeThreadRunning = new std::vector<std::atomic<bool>*>();
	m_activeThreadDone = new std::vector<std::atomic<bool>*>();
	m_counter = 0;

	m_plane = nullptr;
}

ThreadHandler::~ThreadHandler()
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

void ThreadHandler::Reset()
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

	m_activeThreads = new std::vector<std::thread*>();
	m_activeThreadIds = new std::vector<unsigned long long>();
	m_activeThreadRunning = new std::vector<std::atomic<bool>*>();
	m_activeThreadDone = new std::vector<std::atomic<bool>*>();
	m_counter = 0;
}

void ThreadHandler::KillJoinAll()
{
	for (unsigned int i = 0; i < m_activeThreads->size(); i++)
		KillThread((*m_activeThreadIds)[i]);

	for (unsigned int i = 0; i < m_activeThreadDone->size(); i++)
	{
		while (!m_activeThreadDone->at(i)->load())
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	for (uint32_t i = 0; i < m_activeThreads->size(); i++)
	{
		delete m_activeThreads->at(i);
		delete m_activeThreadDone->at(i);
		delete m_activeThreadRunning->at(i);
	}

	m_activeThreads->clear();
	m_activeThreadDone->clear();
	m_activeThreadIds->clear();
	m_activeThreadRunning->clear();
}

void ThreadHandler::SetPlane(Plane* plane)
{
	m_plane = plane;
}

unsigned long long ThreadHandler::SummonThread(unsigned long long stackIndex, void* runtime, void* variables, void* registry, const bool& pass, const double& passValue)
{
	m_counter++;
	m_activeThreadIds->push_back(m_counter);

	std::atomic<bool>* running = new std::atomic<bool>(true);
	m_activeThreadRunning->push_back(running);

	std::atomic<bool>* done = new std::atomic<bool>(false);
	m_activeThreadDone->push_back(done);

	if (pass)
	{
		BlockRuntimeReturn funArgs = ((RuntimeHandler*)runtime)->GetPlane()->GetStack(stackIndex)->GetBlock(0)->GetUsedArgumentsRuntime();
		((VariableHandler*)variables)->StackReal((std::to_string(stackIndex) + "_" + std::to_string(m_counter) + "_" + (*funArgs.Args)[1].Value).c_str());
		((VariableHandler*)variables)->SetReal((std::to_string(stackIndex) + "_" + std::to_string(m_counter) + "_" + (*funArgs.Args)[1].Value).c_str(), passValue);
	}

	std::thread* run = new std::thread(ThreadRuntimeThread, m_plane, stackIndex, running, done, (RuntimeHandler*)runtime, (VariableHandler*)variables, (BlockRegistry*)registry, m_counter);
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

ThreadHandler& ThreadHandler::operator=(const ThreadHandler& other)
{
	return *this;
}
