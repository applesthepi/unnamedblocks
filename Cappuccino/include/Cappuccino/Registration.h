#pragma once
#include "block/ModBlock.h"
#include "ModBlockPass.h"
#include "ExecutionThread.h"
#include "runtime/ModBlockData.h"

#include <shared_mutex>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <SFML/Graphics.h>
#include <chrono>

class Registration
{
public:
	static void Initialize();

	static void RegisterPass(ModBlockPass* pass);
	static void UnRegisterPass(ModBlockPass* pass);

	static void RegisterExecutionThread(ExecutionThread* thr);
	static void UnRegisterExecutionThread(ExecutionThread* thr, bool join);

	static void SetFunctionMain(uint64_t main);
	static void SetFunctionCallCount(uint64_t* functionCallCount);
	static void SetFunctionTotalCount(uint64_t functionTotalCount);
	static void SetCalls(executionFunctionStackList calls);
	static void SetData(ModBlockData** data);
	static void SetBlocks(ModBlock*** blocks);
	static void SetDebug(bool debugBuild);
	static void SetSuper(uint8_t* super, int64_t* superData, void* superMutex);

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
	static bool GlobalPre(PreProcessorData& data);
	static bool GlobalPost(PreProcessorData& data);
	static bool LocalPre(PreProcessorData& data);
	static bool LocalPost(PreProcessorData& data);
	static bool Init(PreProcessorData& preData, ModBlockData** blockData);

	/*
		1 - [ R/D ] stop; kill all

		2 - [ R/D ] break all
		3 - [ R/D ] resume all

		4 - [ __D ] break single thread (idx)
		5 - [ __D ] step single thread
		6 - [ __D ] resume single thread
	*/

	static bool TestSuperBase();
	static bool TestSuperDebug();

	static void CompileDataDebug();
	static void CompileDataRelease();
	static void RunContext();

	static std::mutex m_passesMutex;
	static std::vector<ModBlockPass*> m_passes;
	static std::vector<bool> m_passesFlagged;

	static std::mutex m_executionMutex;
	static std::vector<ExecutionThread*> m_execution;
	static std::vector<bool> m_executionFlagged;
	static std::vector<bool> m_executionJoin;

	static uint64_t m_functionMain;
	static uint64_t* m_functionCallCount;
	static uint64_t m_functionTotalCount;
	static executionFunctionStackList m_calls;
	static ModBlockData** m_data;
	static ModBlock*** m_blocks;

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
	static uint8_t* m_super;
	static int64_t* m_superData;
	static std::mutex* m_superMutex;
	static std::chrono::steady_clock::time_point m_timeBegin;

	static bool m_breakFull;
	static bool m_breakSingle;
	static int64_t m_breakSingleData;

	static std::atomic<bool> m_breakFullResume;
	static std::atomic<bool> m_breakSingleResume;

	static std::vector<std::string> m_variableRegistry; // debug only
};