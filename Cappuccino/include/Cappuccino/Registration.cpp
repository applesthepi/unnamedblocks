#include "Registration.h"

#include <chrono>
#include <vector>
#include <string>
#include <cassert>
#include <functional>

void ThreadUtil()
{
	while (Registration::GetUtilFinished())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		Registration::RunUtilityTick();
	}
}

void Registration::Initialize()
{
	m_utilFinished = false;
	m_debugBuild = true;
	m_utilThread = std::thread(ThreadUtil);
}

void Registration::RegisterPass(ModBlockPass* pass)
{
	std::unique_lock<std::mutex> lock(m_passesMutex);
	m_passes.push_back(pass);
}

void Registration::UnRegisterPass(ModBlockPass* pass)
{
	std::unique_lock<std::mutex> lock(m_passesMutex);

	for (uint64_t i = 0; i < m_passes.size(); i++)
	{
		if (m_passes[i] == pass)
		{
			m_passes.erase(m_passes.begin() + (int64_t)i);
			return;
		}
	}
}

void Registration::RegisterExecutionThread(ExecutionThread* thr)
{
	std::unique_lock<std::mutex> lock(m_executionMutex);

	m_execution.push_back(thr);
}

void Registration::UnRegisterExecutionThread(ExecutionThread* thr)
{
	std::unique_lock<std::mutex> lock(m_executionMutex);

	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (m_execution[i] == thr)
		{
			m_execution.erase(m_execution.begin() + (int64_t)i);
			return;
		}
	}
}

void Registration::SetFunctionMain(uint64_t* main)
{
	m_functionMain = main;
}

void Registration::SetFunctionCallCount(uint64_t* functionCallCount)
{
	m_functionCallCount = functionCallCount;
}

void Registration::SetFunctionTotalCount(uint64_t* functionTotalCount)
{
	m_functionTotalCount = functionTotalCount;
}

void Registration::SetCalls(executionFunctionStackList calls)
{
	m_calls = calls;
}

void Registration::SetData(ModBlockData** data)
{
	m_data = data;
}

void Registration::SetDebug(bool debugBuild)
{
	m_debugBuild = debugBuild;
}

void Registration::EndAll()
{
	m_utilFinished = true;
	m_utilThread.join();

	for (uint64_t i = 0; i < m_execution.size(); i++)
		m_execution[i]->End();
}

std::atomic<bool>& Registration::GetUtilFinished()
{
	return m_utilFinished;
}

void Registration::RunUtilityTick()
{
	std::unique_lock<std::mutex> lock1(m_passesMutex);
	std::unique_lock<std::mutex> lock2(m_executionMutex);

	for (uint64_t i = 0; i < m_passes.size(); i++)
	{
		const std::vector<std::string>& messages = m_passes[i]->GetMessages();

		for (uint64_t a = 0; a < messages.size(); a++)
			printf("%s\n", (messages[a]).c_str());
	}

	m_allDone = IsAllDone();
}

void Registration::Run()
{
	printf("started Cappuccino...\n");

	printf("started parsing data...\n");
	CompileData();
	printf("...finished parsing data\n");

	sfVideoMode mode = { 1920, 1080, 32 };
	m_window = sfRenderWindow_create(mode, "Unnamed Blocks Runtime", sfClose, NULL);

	ModBlockPass* pass = new ModBlockPass(m_window, m_debugBuild);
	RegisterPass(pass);

	UB_ASSERT(m_functionMain != nullptr);
	UB_ASSERT(reinterpret_cast<uint64_t>(m_functionCallCount) > 10);
	UB_ASSERT(m_calls != nullptr);
	ExecutionThread* thr = new ExecutionThread(m_functionMain, m_functionCallCount, m_calls, pass);
	RegisterExecutionThread(thr);

	sfEvent ev;

	while (true)
	{
		while (sfRenderWindow_pollEvent(m_window, &ev))
		{
			if (ev.type == sfEvtClosed)
				sfRenderWindow_close(m_window);
		}

		sfRenderWindow_clear(m_window, { 50, 50, 50, 255 });
		sfRenderWindow_display(m_window);

		if (!sfRenderWindow_isOpen(m_window))
		{
			EndAll();
			break;
		}

		if (m_allDone)
		{
			sfRenderWindow_close(m_window);
			break;
		}
	}

	printf("...stopped Cappuccino\n");
}

bool Registration::IsAllDone()
{
	std::unique_lock<std::mutex> lock(m_executionMutex);

	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (!m_execution[i]->GetFinished())
			return false;
	}

	return true;
}

ModBlockData** Registration::GetData()
{
	return m_data;
}

void Registration::CompileData()
{
	std::vector<std::string> variableRegistry;
	uint64_t variableIdx = 0;

	auto addToRegistry = [&](std::string name)
	{
		for (uint64_t i = 0; i < variableRegistry.size(); i++)
		{
			if (variableRegistry[i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = variableRegistry.size();
		variableRegistry.push_back(name);

		return true;
	};

	for (uint64_t i = 0; i < *m_functionTotalCount; i++)
	{
		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			const std::vector<void*>& data = m_data[i][a].GetData();
			const std::vector<ModBlockDataType>& types = m_data[i][a].GetTypes();
			const std::vector<ModBlockDataInterpretation>& interpretations = m_data[i][a].GetInterpretations();

			std::vector<int64_t> hauledVariables;

			for (uint64_t b = 0; b < data.size(); b++)
			{
				if (types[b] == ModBlockDataType::VAR)
				{
					addToRegistry(*(std::string*)data[b]);
					hauledVariables.push_back(variableIdx);
				}
				else
					hauledVariables.push_back(-1);
			}

			m_data[i][a].HaulData(hauledVariables);
		}
	}
}

std::mutex Registration::m_passesMutex;

std::vector<ModBlockPass*> Registration::m_passes;

std::mutex Registration::m_executionMutex;

std::vector<ExecutionThread*> Registration::m_execution;

uint64_t* Registration::m_functionMain;

uint64_t* Registration::m_functionCallCount;

uint64_t* Registration::m_functionTotalCount;

executionFunctionStackList Registration::m_calls;

ModBlockData** Registration::m_data;

std::atomic<bool> Registration::m_utilFinished;

std::atomic<bool> Registration::m_allDone;

std::thread Registration::m_utilThread;

bool Registration::m_debugBuild;

sfRenderWindow* Registration::m_window;
