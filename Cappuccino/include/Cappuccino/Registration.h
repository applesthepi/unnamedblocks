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

	static void SetFunctionMain(uint64_t main);
	static void SetFunctionCallCount(uint64_t* functionCallCount);
	static void SetFunctionTotalCount(uint64_t functionTotalCount);
	static void SetCalls(executionFunctionStackList calls);
	static void SetData(ModBlockData** data);
	static void SetDebug(bool debugBuild);

	static void EndAll(ModBlockPass* whitelist = nullptr);
	static void Stop();

	static std::atomic<bool>& GetUtilFinished();
	static std::atomic<bool>& GetStop();
	static void SetUtilReturnFinished(bool finished);
	static void RunUtilityTick();
	static void Run();
	static bool IsAllDone();

	static ModBlockData** GetData();
private:
	static void CompileDataDebug();
	static void CompileDataRelease();
	static void RunContext();

	static std::mutex m_passesMutex;
	static std::vector<ModBlockPass*> m_passes;
	static std::vector<bool> m_passesFlagged;

	static std::mutex m_executionMutex;
	static std::vector<ExecutionThread*> m_execution;
	static std::vector<bool> m_executionFlagged;

	static uint64_t m_functionMain;
	static uint64_t* m_functionCallCount;
	static uint64_t m_functionTotalCount;
	static executionFunctionStackList m_calls;
	static ModBlockData** m_data;

	static double* m_variablesReal;
	static bool* m_variablesBool;
	static std::string* m_variablesString;

	static std::mutex m_customRegisterMutex;
	static std::vector<void*> m_customRegister;

	static std::atomic<bool> m_utilFinished;
	static std::atomic<bool> m_utilReturnFinished;
	static std::atomic<bool> m_allDone;
	static std::atomic<bool> m_stop;
	static std::thread m_utilThread;
	static bool m_debugBuild;

	static std::vector<std::string> m_variableRegistry; // debug only
};