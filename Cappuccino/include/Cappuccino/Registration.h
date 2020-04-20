#pragma once
#include "ModBlockPass.h"
#include "ExecutionThread.h"

#include <shared_mutex>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>

class Registration
{
public:
	static void Initialize();

	static void RegisterPass(ModBlockPass* pass);
	static void UnRegisterPass(ModBlockPass* pass);

	static void RegisterExecutionThread(ExecutionThread* thr);
	static void UnRegisterExecutionThread(ExecutionThread* thr);

	static void SetFunctionCallCount(uint64_t* functionCallCount);
	static void SetCalls(executionFunctionStackList calls);
	static void SetDebug(bool debugBuild);

	static bool EndAll();
	static std::atomic<bool>& GetUtilFinished();
	static void RunUtilityTick();
	static void Run();
private:
	static std::mutex m_passesMutex;
	static std::vector<ModBlockPass*> m_passes;

	static std::mutex m_executionMutex;
	static std::vector<ExecutionThread*> m_execution;

	static uint64_t* m_functionCallCount;
	static executionFunctionStackList m_calls;

	static std::atomic<bool> m_utilFinished;
	static std::thread m_utilThread;
	static bool m_debugBuild;
};