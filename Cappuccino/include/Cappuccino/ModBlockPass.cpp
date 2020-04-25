#include "ModBlockPass.h"

#include <cstddef>
#include <cstdio>
#include <locale>
#include <time.h>
#include <cstring>

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
	// Should be enough
	char prefix[100];
	snprintf(prefix, 100, "[%d:%d:%d] [info]", aTime->tm_hour, aTime->tm_min, aTime->tm_sec);

	std::unique_lock<std::mutex> lock(this->m_messagesMutex);
	
	// This is how to get only 1 heap allocation, the reserve call.
	m_messages.emplace_back();
	size_t index = m_messages.size()-1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
}

void ModBlockPass::LogError(const std::string& message)
{
	time_t theTime = time(NULL);
	struct tm* aTime = localtime(&theTime);
	// Should be enough
	char prefix[100];
	snprintf(prefix, 100, "[%d:%d:%d] [error]", aTime->tm_hour, aTime->tm_min, aTime->tm_sec);

	std::unique_lock<std::mutex> lock(this->m_messagesMutex);
	
	// This is how to get only 1 heap allocation, the reserve call.
	m_messages.emplace_back();
	size_t index = m_messages.size()-1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
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

sfRenderWindow* ModBlockPass::GetRenderWindowRelease(ModBlockPass*)
{
	return m_window;
}

void** ModBlockPass::GetDataDebug(ModBlockPass* pass)
{
	pass->LogInfo("retrieving data");
	return m_data;
}

void** ModBlockPass::GetDataRelease(ModBlockPass*)
{
	return m_data;
}
