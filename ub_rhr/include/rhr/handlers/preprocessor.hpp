#pragma once
#include "config.h"

//#include "rhr/stacking/plane.hpp"

//#include <cappuccino/utils.hpp>

//#if LINUX
//#include <dlfcn.h>
//#else
//#include <windows.h>
//#endif

//enum class PreProcessorStatus
//{
//	NOT_READY,
//	RUNNING,
//	DONE,
//	DONE_ERROR
//};

/*

super

0 - nop
1 - stop all
2 - pause all

*/

//class PreProcessor
//{
//public:
//	static void Initialize();
//
//	// building
//	static void Cleanup();
//
//	// executing
//	static void Start(Plane* planeCopy, bool debugBuild);
//	static bool IsFinished();
//	
//	// util
//	static void SetFinished(bool finished);
//	static Plane* GetPlaneCopy();
//	static void SetSuper(u8 super, i16 superData);
//	static u8 GetSuper();
//	static i64 GetSuperData();
//
//	static u8* MakeSuper();
//	static i64* GetMadeData();
//	static std::mutex* GetMadeMutex();
//private:
//	static std::thread m_thread;
//	static std::atomic<bool> m_finished;
//	static Plane* m_planeCopy;
//	static u8* m_super;
//	static i64* m_superData;
//
//	static std::mutex* m_superMutex;
//};