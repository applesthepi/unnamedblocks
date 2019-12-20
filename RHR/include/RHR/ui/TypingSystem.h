#pragma once

#include <vector>
#include <functional>
#include <chrono>

class TypingSystem
{
public:
	static void Initialization();
	static void Update();
	static void AddKeypressRegister(std::function<void(int)>* fun);
	static void RemoveKeypressRegister(std::function<void(int)>* fun);
	static void RunStroke(unsigned int c, bool* currentPress);
	static void RunArrowStroke(unsigned int c, bool* currentPress);

	static std::vector<std::function<void(int)>*>* PressFunctions;
	static bool* WasDown;
	static int* PressRepeatIndex;
#ifdef LINUX
	static std::chrono::time_point<std::chrono::system_clock>* PressRepeatStartTime;
	static std::chrono::time_point<std::chrono::system_clock>* PressRepeatIncTime;
#else
	static std::chrono::time_point<std::chrono::steady_clock>* PressRepeatStartTime;
	static std::chrono::time_point<std::chrono::steady_clock>* PressRepeatIncTime;
#endif
};
