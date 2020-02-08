#pragma once
#include "ThreadHandler.h"
#include "ObjectHandler.h"
#include "VariableHandler.h"
#include "ByteHandler.h"

#include "stacking/Plane.h"

#include <thread>
#include <mutex>
#include <atomic>

class RuntimeHandler
{
public:
	RuntimeHandler(ThreadHandler* thread, ObjectHandler* object, VariableHandler* variable, ByteHandler* byte);

	void Reset();
	void Run(Plane* planeCopy, BlockRegistry* registry);
	void CleanUp();
	void ManualRender();
	int GetScrolled();
	void AddScroll(int value);
	void ResetScrolled();
	int PerformFunctionSearch(std::string functionName);
	const Plane* GetPlane();

	uint64_t MutexCreate();
	bool MutexFree(uint64_t idx);
	bool MutexLockShared(uint64_t idx);
	bool MutexReleaseShared(uint64_t idx);
	bool MutexLockUnique(uint64_t idx);
	bool MutexReleaseUnique(uint64_t idx);

	std::atomic<bool> Running;
	std::atomic<bool> ManualRenderFrame;
	std::atomic<bool> ManualRenderingEnabled;
	sf::RenderWindow* Window;

	RuntimeHandler& operator=(const RuntimeHandler& other);

	const std::vector<StatmentIf>* GetIfStatments(uint32_t stackIdx);
	static std::vector<StatmentIf> ProcessIfStatments(Stack* stack, bool muteErrors = false);
private:
	Plane* m_planeCopy;
	std::thread* m_runningThread;
	std::mutex m_renderMutex;
	std::mutex m_scrollMutex;
	int m_scrolled;
	uint64_t m_appMutexCount;
	std::vector<std::shared_mutex*> m_appMutex;
	std::vector<uint64_t> m_appMutexIdx;
	std::vector<unsigned int> m_stackIndices;
	std::vector<std::string> m_stackFunctions;
	std::vector<std::vector<StatmentIf>> m_stackFunctionIfStatments;

	ThreadHandler* m_threadHandler;
	ObjectHandler* m_objectHandler;
	VariableHandler* m_variableHandler;
	ByteHandler* m_byteHandler;
};
