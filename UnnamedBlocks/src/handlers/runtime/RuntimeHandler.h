#pragma once

#include "stacking/Plane.h"

#include <thread>
#include <mutex>

class RuntimeHandler
{
public:
	static void Run(Plane* planeCopy);
	static void CleanUp();
	static void ManualRender();
	static int GetScrolled();
	static void AddScroll(int value);
	static void ResetScrolled();
	static int PerformFunctionSearch(std::string functionName);

	static bool Running;
	static bool ManualRenderFrame;
	static bool ManualRenderingEnabled;
	static sf::RenderWindow* Window;
private:
	static Plane* m_planeCopy;
	static std::thread* m_runningThread;
	static std::mutex m_renderMutex;
	static std::mutex m_scrollMutex;
	static int m_scrolled;
	static std::vector<unsigned int> m_stackIndices;
	static std::vector<std::string> m_stackFunctions;
};
