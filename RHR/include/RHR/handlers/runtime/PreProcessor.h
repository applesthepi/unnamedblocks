#pragma once

#ifdef POSIX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#include "PreProcessorTranslationUnit.h"

#include <mutex>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <atomic>

enum class PreProcessorStatus
{
	NOT_READY,
	RUNNING,
	DONE,
	DONE_ERROR
};

class PreProcessor
{
public:
	// preprocessor translation unit building
	static void Cleanup();
	static const uint64_t InitializeTranslationUnit(const Stack* stack, BlockRegistry* blockRegistry);
	static PreProcessorTranslationUnitStatus GetTranslationUnitStatus(const uint64_t& idx);
	
	// building
	static void BeginBuild(const bool& releaseBuild);
	static PreProcessorStatus GetStatus();

	// executing
	static void Start();
	static const bool IsFinished();
	static const bool IsRunning();
	
	// util
	static void SetStatus(PreProcessorStatus status);
	static void SetFinished(const bool& finished);
	static void SetFinishedStatus(const int& finishedStatus);
	static void SetRunning(const bool& running);
	static void SetDll(LPCWSTR path);

	static HINSTANCE* GetDll();
private:
	static std::vector<PreProcessorTranslationUnit> m_units;

	static std::mutex m_statusMutex;
	static PreProcessorStatus m_status;

	static std::thread m_thread;
	static std::atomic<bool> m_finished;
	static std::atomic<bool> m_running;
	static int m_finishedStatus;
	static HINSTANCE* m_dll;
};
