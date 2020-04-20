#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <shared_mutex>

class ModBlockPass
{
public:
	ModBlockPass(sf::RenderWindow* window, void** data, bool debugMode);

	void LogInfo(const std::string& message);
	void LogError(const std::string& message);

	const std::vector<std::string>& GetMessages();
	
	// user data

	sf::RenderWindow* GetRenderWindow();
	void SetData(void** data);
	void** GetData();
private:
	// user data interactions
	sf::RenderWindow* (ModBlockPass::*m_getRenderWindow)(ModBlockPass* pass);
	sf::RenderWindow* GetRenderWindowDebug(ModBlockPass* pass);
	sf::RenderWindow* GetRenderWindowRelease(ModBlockPass* pass);

	void** (ModBlockPass::*m_getData)(ModBlockPass* pass);
	void** GetDataDebug(ModBlockPass* pass);
	void** GetDataRelease(ModBlockPass* pass);

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;

	// user data

	sf::RenderWindow* m_window;
	void** m_data;
};