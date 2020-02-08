#pragma once
#include "stacking/Plane.h"

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class ThreadHandler
{
public:
	ThreadHandler();
	~ThreadHandler();

	void Reset();
	void KillJoinAll();
	void SetPlane(Plane* plane);

	unsigned long long SummonThread(unsigned long long stackIndex, void* runtime, void* variables, void* registry, const bool& pass, const double& passValue);
	bool KillThread(unsigned long long threadId);

	ThreadHandler& operator=(const ThreadHandler& other);
private:
	std::vector<std::thread*>* m_activeThreads;
	std::vector<unsigned long long>* m_activeThreadIds;
	std::vector<std::atomic<bool>*>* m_activeThreadRunning;
	std::vector<std::atomic<bool>*>* m_activeThreadDone;
	unsigned long long m_counter;
	Plane* m_plane;
};