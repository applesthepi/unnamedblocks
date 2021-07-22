#pragma once
#include "config.h"

#include "runtime/ModBlockData.hpp"

#include "Utils.hpp"

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
	const std::vector<uint64_t>* VariablesRealCount;
	const std::vector<uint64_t>* VariablesBoolCount;
	const std::vector<uint64_t>* VariablesStringCount;

	std::mutex* CustomRegisterMutex;
	std::vector<void*>* CustomRegister;
	void(*Stop)();
	std::chrono::steady_clock::time_point* BeginTime;

	// debug only

	uint64_t DataSize;
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
	CAP_DLL void SetCallstackStack(std::vector<uint64_t>* callstack);
	CAP_DLL void SetCallstackBlock(std::vector<uint64_t>* callstack);
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

	CAP_DLL double& GetReal(uint64_t idx);
	CAP_DLL bool& GetBool(uint64_t idx);
	CAP_DLL std::string& GetString(uint64_t idx);

	CAP_DLL void* GetPreData(uint64_t idx);
	CAP_DLL std::chrono::steady_clock::time_point* GetBeginTime();

	CAP_DLL void Stop();

	CAP_DLL void AddCallstack(uint64_t stack, uint64_t block, bool special = true);
	CAP_DLL void PopCallstack();

	CAP_DLL void SetStackIdx(uint64_t idx);
	CAP_DLL void SetBlockIdx(uint64_t idx);

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

	CAP_DLL uint64_t CustomPut(void* mem);
	CAP_DLL void* CustomGet(uint64_t idx);
	CAP_DLL void CustomFree(uint64_t idx, bool deallocate = true);
private:
	void UpdateLocations();

	// user data interactions

	double& (ModBlockPass::* m_getReal)(uint64_t idx);
	double& GetRealDebug(uint64_t idx);
	double& GetRealRelease(uint64_t idx);

	bool& (ModBlockPass::* m_getBool)(uint64_t idx);
	bool& GetBoolDebug(uint64_t idx);
	bool& GetBoolRelease(uint64_t idx);

	std::string& (ModBlockPass::* m_getString)(uint64_t idx);
	std::string& GetStringDebug(uint64_t idx);
	std::string& GetStringRelease(uint64_t idx);

	void* (ModBlockPass::* m_getPreData)(uint64_t idx);
	void* GetPreDataDebug(uint64_t idx);
	void* GetPreDataRelease(uint64_t idx);

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;
	void(*m_stop)();
	std::chrono::steady_clock::time_point* m_beginTime;
	std::mt19937_64 m_random;

	// debug only
	std::vector<std::string>* m_variableRegistry;

	// user data

	ModBlockData** m_data;

	std::vector<const uint64_t**> m_dataStackIdx;
	std::vector<void***> m_dataStackPre;
	std::vector<double*> m_dataStackReal;
	std::vector<bool*> m_dataStackBool;
	std::vector<std::string*> m_dataStackString;

	const uint64_t** m_activeIdx;
	void*** m_activePre;
	double* m_activeReal;
	bool* m_activeBool;
	std::string* m_activeString;
	
	std::vector<bool> m_stackingSpecial;
	std::vector<double*> m_stackingReal;
	std::vector<bool*> m_stackingBool;
	std::vector<std::string*> m_stackingString;

	std::vector<uint64_t*> m_preDataCount;
	const std::vector<uint64_t>* m_variablesRealCount;
	const std::vector<uint64_t>* m_variablesBoolCount;
	const std::vector<uint64_t>* m_variablesStringCount;

	std::mutex* m_customRegistrerMutex;
	std::vector<void*>* m_customRegister;

	std::vector<uint64_t>* m_callstackStackIdx;
	std::vector<uint64_t>* m_callstackBlockIdx;

	void(***m_localCallstack)(ModBlockPass*);
	void(***m_calls)(ModBlockPass*);

	std::vector<std::function<void(ModBlockPass*)>*> m_deallocationCalls;

	bool* m_successful;
	std::atomic<bool>* m_finished;
};
