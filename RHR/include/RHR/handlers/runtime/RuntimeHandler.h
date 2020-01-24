#pragma once
#include "ThreadHandler.h"
#include "ObjectHandler.h"
#include "VariableHandler.h"
#include "ByteHandler.h"

#include "stacking/Plane.h"

#include <thread>
#include <mutex>

class RuntimeHandler
{
public:
	RuntimeHandler(ThreadHandler* thread, ObjectHandler* object, VariableHandler* variable, ByteHandler* byte);

	void Reset();
	void Run(Plane* planeCopy);
	void CleanUp();
	void ManualRender();
	int GetScrolled();
	void AddScroll(int value);
	void ResetScrolled();
	int PerformFunctionSearch(std::string functionName);

	bool Running;
	bool ManualRenderFrame;
	bool ManualRenderingEnabled;
	sf::RenderWindow* Window;

	RuntimeHandler& operator=(const RuntimeHandler& other);
private:
	Plane* m_planeCopy;
	std::thread* m_runningThread;
	std::mutex m_renderMutex;
	std::mutex m_scrollMutex;
	int m_scrolled;
	std::vector<unsigned int> m_stackIndices;
	std::vector<std::string> m_stackFunctions;

	ThreadHandler* m_threadHandler;
	ObjectHandler* m_objectHandler;
	VariableHandler* m_variableHandler;
	ByteHandler* m_byteHandler;
};
