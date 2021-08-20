#pragma once
#include "config.h"
#include "cap_config.h"

#include "cappuccino/utils.hpp"
#include "cappuccino/mod_block/block.hpp"
#include "cappuccino/mod_block/pass.hpp"
#include "cappuccino/execution_thread.hpp"
#include "cappuccino/mod_block/data.hpp"

#if !LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

class Registration
{
public:
	static void Initialize();

	static CAP_DLL void RegisterPass(ModBlockPass* pass);
	static CAP_DLL void UnRegisterPass(ModBlockPass* pass);

	static CAP_DLL void RegisterExecutionThread(ExecutionThread* thr);
	static CAP_DLL void UnRegisterExecutionThread(ExecutionThread* thr, bool join);

	static void SetFunctionMain(u64 main);
	static void SetFunctionCallCount(u64* functionCallCount);
	static void SetFunctionTotalCount(u64 functionTotalCount);
	static void SetCalls(executionFunctionStackList calls);
	static void SetData(ModBlockData** data);
	static void SetBlocks(ModBlock*** blocks);
	static void SetDebug(bool debugBuild);
	static void SetSuper(u8* super, i64* superData, void* superMutex);

	static void EndAll(ModBlockPass* whitelist = nullptr);
	static CAP_DLL void Stop();

	static std::atomic<bool>& GetUtilFinished();
	static std::atomic<bool>& GetStop();
	static CAP_DLL u64* GetFunctionCallCount();
	static CAP_DLL u64 GetFunctionTotalCount();
	static CAP_DLL executionFunctionStackList GetCalls();
	static void SetUtilReturnFinished(bool finished);
	static void RunUtilityTick();
	static void Run();
	static bool IsAllDone();

	static CAP_DLL const std::vector<f64*>& GetRealTemplate();
	static CAP_DLL const std::vector<bool*>& GetBoolTemplate();
	static CAP_DLL const std::vector<std::string*>& GetStringTemplate();

	static CAP_DLL const std::vector<u64>* GetRealCount();
	static CAP_DLL const std::vector<u64>* GetBoolCount();
	static CAP_DLL const std::vector<u64>* GetStringCount();

	static CAP_DLL std::mutex* GetCustomMutex();
	static CAP_DLL std::vector<void*>* GetCustomRegistry();
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

	static u64 m_functionMain;
	static u64* m_functionCallCount;
	static u64 m_functionTotalCount;
	static executionFunctionStackList m_calls;
	static ModBlockData** m_data;
	static ModBlock*** m_blocks;

	static std::vector<u64> m_variableRealCount;
	static std::vector<u64> m_variableBoolCount;
	static std::vector<u64> m_variableStringCount;

	static std::vector<f64*> m_variableRealTemplate;
	static std::vector<bool*> m_variableBoolTemplate;
	static std::vector<std::string*> m_variableStringTemplate;

	static std::mutex m_customRegisterMutex;
	static std::vector<void*> m_customRegister;

	static std::atomic<bool> m_utilFinished;
	static std::atomic<bool> m_utilReturnFinished;
	static std::atomic<bool> m_allDone;
	static std::atomic<bool> m_stop;
	static std::thread m_utilThread;
	static bool m_debugBuild;
	static u8* m_super;
	static i64* m_superData;
	static std::mutex* m_superMutex;
	static std::chrono::steady_clock::time_point m_timeBegin;

	static bool m_breakFull;
	static bool m_breakSingle;
	static i64 m_breakSingleData;

	static std::atomic<bool> m_breakFullResume;
	static std::atomic<bool> m_breakSingleResume;

	static std::vector<std::vector<std::string>> m_variableRegistry;
	static std::vector<u64> m_variableRegistryOffsets;
};