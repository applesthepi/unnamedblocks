#pragma once
#include "config.h"

#include <stdint.h>
#include <shared_mutex>
#include <vector>
#include <string>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/System/Mutex.h>
#include <chrono>

#ifndef LINUX
	#ifdef __CAP
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
	ModBlockPassInitializer();

	bool DebugMode;

	// user data

	void** Data;
	double* VariablesReal;
	bool* VariablesBool;
	std::string* VariablesString;

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
	CAP_DLL void SetData(void** data);

	// debug only
	CAP_DLL void SetDataSize(const uint64_t& size);

	// ====================================================
	// user calls
	// ====================================================

	CAP_DLL void LogDebug(const std::string& message);

	CAP_DLL void LogInfo(const std::string& message);
	CAP_DLL void LogWarning(const std::string& message);
	CAP_DLL void LogError(const std::string& message, const LoggerFatality& fatality);

	CAP_DLL void** GetData(const uint64_t& idx);

	CAP_DLL double& GetVariableReal(const uint64_t& idx);
	CAP_DLL bool& GetVariableBool(const uint64_t& idx);
	CAP_DLL std::string& GetVariableString(const uint64_t& idx);

	CAP_DLL void Stop();

	// ====================================================
	// custom allocations
	// ====================================================

	// do NOT use "malloc" or any other "C" style allocation; you MUST use "new" to allocate
	// do NOT allocate an array; you MUST allocate a single object
	// minimize the number of custom allocations you do
	// calling "CustomFree" deletes the object, but its index persists!
	CAP_DLL const uint64_t CustomPut(void* mem);
	CAP_DLL void* CustomGet(const uint64_t& idx);
	CAP_DLL void CustomFree(const uint64_t& idx);
private:
	// user data interactions

	void** (ModBlockPass::* m_getData)(const uint64_t& idx);
	void** GetDataDebug(const uint64_t& idx);
	void** GetDataRelease(const uint64_t& idx);

	double& (ModBlockPass::* m_getVaraibleReal)(const uint64_t& idx);
	double& GetVariableRealDebug(const uint64_t& idx);
	double& GetVariableRealRelease(const uint64_t& idx);

	bool& (ModBlockPass::* m_getVariableBool)(const uint64_t& idx);
	bool& GetVariableBoolDebug(const uint64_t& idx);
	bool& GetVariableBoolRelease(const uint64_t& idx);

	std::string& (ModBlockPass::* m_getVariableString)(const uint64_t& idx);
	std::string& GetVariableStringDebug(const uint64_t& idx);
	std::string& GetVariableStringRelease(const uint64_t& idx);

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;
	void(*m_stop)();
	std::chrono::steady_clock::time_point* m_beginTime;

	// debug only
	uint64_t m_dataSize;
	// debug only
	std::vector<std::string>* m_variableRegistry;

	// user data

	void** m_data;
	
	double* m_variablesReal;
	bool* m_variablesBool;
	std::string* m_variablesString;
	std::mutex* m_customRegistrerMutex;
	std::vector<void*>* m_customRegister;
};
