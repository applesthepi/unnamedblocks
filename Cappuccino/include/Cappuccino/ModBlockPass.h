#pragma once
#include <stdint.h>
#include <SFML/Graphics.h>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <string>

class ModBlockPass
{
public:
	ModBlockPass(sfRenderWindow* window, bool debugMode);

	void LogInfo(const std::string& message);
	void LogError(const std::string& message);
	
	const std::vector<std::string>& GetMessages();

	// user data

	sfRenderWindow* GetRenderWindow();
	void SetData(void** data);
	void** GetData();
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