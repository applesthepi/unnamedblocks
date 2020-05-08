#pragma once
#include "config.h"
#include "RHR/stacking/Plane.h"
#include <mutex>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>

#ifdef POSIX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

enum class PreProcessorStatus
{
	NOT_READY,
	RUNNING,
	DONE,
	DONE_ERROR
};

/*

super

0 - nop
1 - stop all
2 - pause all

*/

class PreProcessor
{
public:
	static void Initialize();

	// building
	static void Cleanup();

	// executing
	static void Start(Plane* planeCopy, BlockRegistry* registry, bool debugBuild);
	static bool IsFinished();
	
	// util
	static void SetFinished(const bool& finished);
	static Plane* GetPlaneCopy();
	static BlockRegistry* GetRegistry();
	static void SetSuper(const uint8_t& super, const int64_t& superData);
	static const uint8_t GetSuper();
	static const int64_t GetSuperData();

	static uint8_t* MakeSuper();
	static int64_t* GetMadeData();
	static std::mutex* GetMadeMutex();
private:
	static std::thread m_thread;
	static std::atomic<bool> m_finished;
	static Plane* m_planeCopy;
	static BlockRegistry* m_registry;
	static uint8_t* m_super;
	static int64_t* m_superData;

	static std::mutex* m_superMutex;
};
