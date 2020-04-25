#pragma once
#include "config.h"
#include <stdint.h>
#include <SFML/Graphics.h>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <string>
#ifndef LINUX
	#ifdef __CAP
		#define CAP_DLL __declspec(dllexport)
	#else
		#define CAP_DLL __declspec(dllimport)
	#endif
#else
	#define CAP_DLL 
#endif
class ModBlockPass
{
public:
	CAP_DLL ModBlockPass(sfRenderWindow* window, bool debugMode);

	CAP_DLL void LogInfo(const std::string& message);
	CAP_DLL void LogError(const std::string& message);
	
	CAP_DLL const std::vector<std::string>& GetMessages();

	// user data

	CAP_DLL sfRenderWindow* GetRenderWindow();
	CAP_DLL void SetData(void** data);
	CAP_DLL void** GetData();
private:
	// user data interactions
	sfRenderWindow* (ModBlockPass::* m_getRenderWindow)(ModBlockPass* pass);
	sfRenderWindow* GetRenderWindowDebug(ModBlockPass* pass);
	sfRenderWindow* GetRenderWindowRelease(ModBlockPass* pass);

	void** (ModBlockPass::* m_getData)(ModBlockPass* pass);
	void** GetDataDebug(ModBlockPass* pass);
	void** GetDataRelease(ModBlockPass* pass);

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;

	// user data

	sfRenderWindow* m_window;
	void** m_data;
};
