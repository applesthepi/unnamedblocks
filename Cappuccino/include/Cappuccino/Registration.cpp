#include "Registration.h"
#include <chrono>
#include <vector>
#include <string>
#include <cassert>

void ThreadUtil()
{
	while (Registration::GetUtilFinished())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Registration::RunUtilityTick();
	}
}

void Registration::Initialize()
{
	sfVideoMode mode = { 1920, 1080, 32 };
	m_window = sfRenderWindow_create(mode, "Unnamed Blocks Runtime", sfClose, NULL);

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

void Registration::SetCalls(executionFunctionStackList calls)
{
	m_calls = calls;
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
	std::unique_lock<std::mutex> lock(m_passesMutex);

	for (uint64_t i = 0; i < m_passes.size(); i++)
	{
		const std::vector<std::string>& messages = m_passes[i]->GetMessages();

		for (uint64_t a = 0; a < messages.size(); a++)
			printf("%s\n", (messages[a]).c_str());
	}
}

void Registration::Run()
{
	printf("run called\n");

	ModBlockPass* pass = new ModBlockPass(m_window, m_debugBuild);
	RegisterPass(pass);

	UB_ASSERT(m_functionMain != nullptr);
	UB_ASSERT(reinterpret_cast<uint64_t>(m_functionCallCount) > 10);
	UB_ASSERT(m_calls != nullptr);
	ExecutionThread* thr = new ExecutionThread(m_functionMain, m_functionCallCount, m_calls, pass);
	RegisterExecutionThread(thr);
}

std::mutex Registration::m_passesMutex;

std::vector<ModBlockPass*> Registration::m_passes;

std::mutex Registration::m_executionMutex;

std::vector<ExecutionThread*> Registration::m_execution;

uint64_t* Registration::m_functionMain;

uint64_t* Registration::m_functionCallCount;

executionFunctionStackList Registration::m_calls;

std::atomic<bool> Registration::m_utilFinished;

std::thread Registration::m_utilThread;

bool Registration::m_debugBuild;

sfRenderWindow* Registration::m_window;
