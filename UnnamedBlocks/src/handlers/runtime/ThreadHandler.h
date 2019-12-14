#pragma once

#include "stacking/Plane.h"

#include <vector>
#include <thread>
#include <mutex>

class ThreadHandler
{
public:
	static void Alloc(Plane* plane);
	static void Dealloc();
	static void KillJoinAll();

	static unsigned long long SummonThread(unsigned long long stackIndex);
	static bool KillThread(unsigned long long threadId);
private:
	static std::vector<std::thread*>* m_activeThreads;
	static std::vector<unsigned long long>* m_activeThreadIds;
	static std::vector<bool*>* m_activeThreadRunning;
	static std::vector<std::mutex*>* m_activeThreadRunningMutex;
	static std::vector<bool*>* m_activeThreadDone;
	static std::vector<std::mutex*>* m_activeThreadDoneMutex;
	static unsigned long long m_counter;
	static Plane* m_plane;
};