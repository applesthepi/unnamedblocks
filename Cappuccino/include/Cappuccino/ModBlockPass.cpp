#include "ModBlockPass.h"

#include <time.h>

ModBlockPass::ModBlockPass(sfRenderWindow* window, bool debugMode)
	:m_window(window), m_data(nullptr)
{
	if (debugMode)
	{
		m_getRenderWindow = &ModBlockPass::GetRenderWindowDebug;
		m_getData = &ModBlockPass::GetDataDebug;
	}
	else
	{
		m_getRenderWindow = &ModBlockPass::GetRenderWindowRelease;
		m_getData = &ModBlockPass::GetDataRelease;
	}
}

sfRenderWindow* ModBlockPass::GetRenderWindow()
{
	return (this->*(m_getRenderWindow))(this);
}

void ModBlockPass::SetData(void** data)
{
	m_data = data;
}

void** ModBlockPass::GetData()
{
	return (this->*(m_getData))(this);
}

void ModBlockPass::LogInfo(const std::string& message)
{
	time_t theTime = time(NULL);
	struct tm* aTime = localtime(&theTime);

	std::unique_lock<std::mutex>(m_messagesMutex);
	m_messages.push_back("[" + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec) + "] [info]" + message);
}

void ModBlockPass::LogError(const std::string& message)
{
	time_t theTime = time(NULL);
	struct tm* aTime = localtime(&theTime);

	std::unique_lock<std::mutex>(m_messagesMutex);
	m_messages.push_back("[" + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec) + "] [error]" + message);
}

const std::vector<std::string>& ModBlockPass::GetMessages()
{
	return m_messages;
}

sfRenderWindow* ModBlockPass::GetRenderWindowDebug(ModBlockPass* pass)
{
	pass->LogInfo("passing RenderWindow");
	return m_window;
}

sfRenderWindow* ModBlockPass::GetRenderWindowRelease(ModBlockPass* pass)
{
	return m_window;
}

void** ModBlockPass::GetDataDebug(ModBlockPass* pass)
{
	pass->LogInfo("retrieving data");
	return m_data;
}

void** ModBlockPass::GetDataRelease(ModBlockPass* pass)
{
	return m_data;
}