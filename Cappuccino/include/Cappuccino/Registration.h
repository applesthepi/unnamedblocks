#pragma once
#include "ModBlockPass.h"
#include "ExecutionThread.h"
#include "runtime/ModBlockData.h"

#include <shared_mutex>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <SFML/Graphics.h>

class Registration
{
public:
	static void Initialize();

	static void RegisterPass(ModBlockPass* pass);
	static void UnRegisterPass(ModBlockPass* pass);

	static void RegisterExecutionThread(ExecutionThread* thr);
	static void UnRegisterExecutionThread(ExecutionThread* thr);

	static void SetFunctionMain(uint64_t* main);
	static void SetFunctionCallCount(uint64_t* functionCallCount);
	static void SetFunctionTotalCount(uint64_t* functionTotalCount);
	static void SetCalls(executionFunctionStackList calls);
	static void SetData(ModBlockData** data);
	static void SetDebug(bool debugBuild);

	static void EndAll();
	static std::atomic<bool>& GetUtilFinished();
	static void RunUtilityTick();
	static void Run();
	static bool IsAllDone();

	static ModBlockData** GetData();
private:
	static void CompileData();

	static std::mutex m_passesMutex;
	static std::vector<ModBlockPass*> m_passes;

	static std::mutex m_executionMutex;
	static std::vector<ExecutionThread*> m_execution;

	static uint64_t* m_functionMain;
	static uint64_t* m_functionCallCount;
	static uint64_t* m_functionTotalCount;
	static executionFunctionStackList m_calls;
	static ModBlockData** m_data;

	static std::atomic<bool> m_utilFinished;
	static std::atomic<bool> m_allDone;
	static std::thread m_utilThread;
	static bool m_debugBuild;
	static sfRenderWindow* m_window;
};