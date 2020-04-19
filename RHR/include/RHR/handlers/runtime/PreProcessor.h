#pragma once

//#include "config.h"

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

class PreProcessor
{
public:
	// building
	static void Cleanup();

	// executing
	static void Start();
	static const bool IsFinished();
	
	// util
	static void SetFinished(const bool& finished);
private:
	static std::thread m_thread;
	static std::atomic<bool> m_finished;
};
