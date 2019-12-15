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

	static bool Running;
	static bool ManualRenderingEnabled;
	static sf::RenderWindow* Window;
private:
	static Plane* m_planeCopy;
	static std::thread* m_runningThread;
	static std::mutex m_renderMutex;
};
