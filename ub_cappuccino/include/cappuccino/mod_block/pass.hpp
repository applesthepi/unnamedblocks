#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"
#include "cappuccino/mod_block/data.hpp"

#if !LINUX
	#ifdef CAPPUCCINO
		#define CAP_DLL __declspec(dllexport)
	#else
		#define CAP_DLL __declspec(dllimport)
	#endif
#else
	#define CAP_DLL
#endif

class ModBlockPassInitializer
{
public:
	CAP_DLL ModBlockPassInitializer();

	bool DebugMode;

	// user data

	void** Data;
	const std::vector<u64>* VariablesRealCount;
	const std::vector<u64>* VariablesBoolCount;
	const std::vector<u64>* VariablesStringCount;

	std::mutex* CustomRegisterMutex;
	std::vector<void*>* CustomRegister;
	void(*Stop)();
	std::chrono::steady_clock::time_point* BeginTime;

	// debug only

	u64 DataSize;
	std::vector<std::string>* VariableRegistry;
};

enum LoggerFatality
{
	OK, BREAK, ABORT
	/*
	OK    - proceeds as normal
	
	BREAK - breaks in the editor; the user can resume

	ABORT - "bork bork nom nom", that k9 got a little to excited!
	        terminates the runtime environment
	*/
};

class ModBlockPass
{
public:
	// ====================================================
	// engine calls
	// ====================================================

	CAP_DLL ModBlockPass(const ModBlockPassInitializer& init);

	CAP_DLL const std::vector<std::string>& PullMessages();
	CAP_DLL void ReturnMessages();
	CAP_DLL void SetData(ModBlockData** data);
	CAP_DLL void SetCallstackStack(std::vector<u64>* callstack);
	CAP_DLL void SetCallstackBlock(std::vector<u64>* callstack);
	CAP_DLL void SetSuccessful(bool* successful);
	CAP_DLL void SetFinished(std::atomic<bool>* finished);
	CAP_DLL void SetCallstackLocal(void(***localCallstack)(ModBlockPass*));
	CAP_DLL void SetCalls(void(***calls)(ModBlockPass*));
	CAP_DLL void PerformDeallocationCallbacks();
	//CAP_DLL void PerformLocationUpdate();

	// ====================================================
	// user calls
	// ====================================================

	CAP_DLL void LogDebug(const std::string& message);
	CAP_DLL void LogInfo(const std::string& message);
	CAP_DLL void LogWarning(const std::string& message);
	CAP_DLL void LogError(const std::string& message, const LoggerFatality& fatality);

	CAP_DLL f64& GetReal(u64 idx);
	CAP_DLL bool& GetBool(u64 idx);
	CAP_DLL std::string& GetString(u64 idx);

	CAP_DLL void* GetPreData(u64 idx);
	CAP_DLL std::chrono::steady_clock::time_point* GetBeginTime();

	CAP_DLL void Stop();

	CAP_DLL void AddCallstack(u64 stack, u64 block, bool special = true);
	CAP_DLL void PopCallstack();

	CAP_DLL void SetStackIdx(u64 idx);
	CAP_DLL void SetBlockIdx(u64 idx);

	CAP_DLL std::mt19937_64& GetRandomGenerator();

	// deallocation callbacks MUST be on the heap.
	// do not deallocate the callback yourself, Cappuccino will do that.

	CAP_DLL void AddDeallocation(std::function<void(ModBlockPass*)>* dealloc);
	CAP_DLL void RemoveDeallocation(std::function<void(ModBlockPass*)>* dealloc);

	// ====================================================
	// custom allocations
	// ====================================================

	// do NOT use "malloc" or any other "C" style allocation; you MUST use "new" to allocate!
	// do NOT allocate an array; you MUST allocate a single object.
	// minimize the number of custom allocations you do.
	// calling "CustomFree" deletes the object, but its index persists!

	// after "CustomPut", it must be either persist forever, or you must
	//		call "CustomFree", to deallocate it. If The object is freed externally before
	//		Cappuccino ends, then you must call "CustomFree" with the parameter "deallocate" set to false.
	//		This will ensure it does not get freed at the end.

	CAP_DLL u64 CustomPut(void* mem);
	CAP_DLL void* CustomGet(u64 idx);
	CAP_DLL void CustomFree(u64 idx, bool deallocate = true);
private:
	void UpdateLocations();

	// user data interactions

	f64& (ModBlockPass::* m_getReal)(u64 idx);
	f64& GetRealDebug(u64 idx);
	f64& GetRealRelease(u64 idx);

	bool& (ModBlockPass::* m_getBool)(u64 idx);
	bool& GetBoolDebug(u64 idx);
	bool& GetBoolRelease(u64 idx);

	std::string& (ModBlockPass::* m_getString)(u64 idx);
	std::string& GetStringDebug(u64 idx);
	std::string& GetStringRelease(u64 idx);

	void* (ModBlockPass::* m_getPreData)(u64 idx);
	void* GetPreDataDebug(u64 idx);
	void* GetPreDataRelease(u64 idx);

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;
	void(*m_stop)();
	std::chrono::steady_clock::time_point* m_beginTime;
	std::mt19937_64 m_random;

	// debug only
	std::vector<std::string>* m_variableRegistry;

	// user data

	ModBlockData** m_data;

	std::vector<const u64**> m_dataStackIdx;
	std::vector<void***> m_dataStackPre;
	std::vector<f64*> m_dataStackReal;
	std::vector<bool*> m_dataStackBool;
	std::vector<std::string*> m_dataStackString;

	const u64** m_activeIdx;
	void*** m_activePre;
	f64* m_activeReal;
	bool* m_activeBool;
	std::string* m_activeString;
	
	std::vector<bool> m_stackingSpecial;
	std::vector<f64*> m_stackingReal;
	std::vector<bool*> m_stackingBool;
	std::vector<std::string*> m_stackingString;

	std::vector<u64*> m_preDataCount;
	const std::vector<u64>* m_variablesRealCount;
	const std::vector<u64>* m_variablesBoolCount;
	const std::vector<u64>* m_variablesStringCount;

	std::mutex* m_customRegistrerMutex;
	std::vector<void*>* m_customRegister;

	std::vector<u64>* m_callstackStackIdx;
	std::vector<u64>* m_callstackBlockIdx;

	void(***m_localCallstack)(ModBlockPass*);
	void(***m_calls)(ModBlockPass*);

	std::vector<std::function<void(ModBlockPass*)>*> m_deallocationCalls;

	bool* m_successful;
	std::atomic<bool>* m_finished;
};
